from flask import Flask, render_template, request, jsonify
import cv2
import numpy as np
import os
import time

app = Flask(__name__)
UPLOAD_FOLDER = 'uploads'
EDGE_FOLDER = 'static/edges'

# Create folders if not exist
os.makedirs(UPLOAD_FOLDER, exist_ok=True)
os.makedirs(EDGE_FOLDER, exist_ok=True)

# Fixed filenames (used throughout)
original_filename = 'original.jpg'
sample_filename = 'sample.jpg'

def process_image(input_path, output_path):
    img = cv2.imread(input_path)
    if img is None:
        raise ValueError(f"Image at {input_path} could not be read.")
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 100, 200)
    cv2.imwrite(output_path, edges)
    return edges

def compare_edges(img1, img2):
    if img1.shape != img2.shape:
        img2 = cv2.resize(img2, (img1.shape[1], img1.shape[0]))
    diff = cv2.absdiff(img1, img2)
    score = np.sum(diff) / 255
    threshold = 280000  # Tune this as per your real use-case
    is_original = score < threshold
    return is_original, score

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/analyze', methods=['POST'])
def analyze():
    try:
        original = request.files.get('original')
        sample = request.files.get('sample')

        if not original or not sample:
            return jsonify({'message': '❌ Please upload both images before analyzing.'}), 400

        original_path = os.path.join(UPLOAD_FOLDER, original_filename)
        sample_path = os.path.join(UPLOAD_FOLDER, sample_filename)
        edge_original_path = os.path.join(EDGE_FOLDER, 'original_edge.jpg')
        edge_sample_path = os.path.join(EDGE_FOLDER, 'sample_edge.jpg')

        original.save(original_path)
        sample.save(sample_path)

        process_image(original_path, edge_original_path)
        process_image(sample_path, edge_sample_path)

        return jsonify({'message': '✅ Images uploaded and processed successfully.'})

    except Exception as e:
        print("Error in /analyze:", str(e))
        return jsonify({'message': '❌ Error during image upload or processing.'}), 500

@app.route('/trigger_analysis', methods=['POST'])
def trigger_analysis():
    try:
        original_path = os.path.join(UPLOAD_FOLDER, original_filename)
        sample_path = os.path.join(UPLOAD_FOLDER, sample_filename)
        edge_original_path = os.path.join(EDGE_FOLDER, 'original_edge.jpg')
        edge_sample_path = os.path.join(EDGE_FOLDER, 'sample_edge.jpg')

        # Check if images exist
        if not os.path.exists(original_path) or not os.path.exists(sample_path):
            return jsonify({
                'message': '❌ Images not uploaded yet. Please upload them first.',
                'result': -1
            }), 400

        # Optional: Ensure recent uploads (within 5 minutes)
        now = time.time()
        recent_limit = 300  # 5 minutes
        if any(now - os.path.getmtime(p) > recent_limit for p in [original_path, sample_path]):
            return jsonify({
                'message': '❌ Uploaded images are outdated. Please re-upload.',
                'result': -1
            }), 400

        original_edges = process_image(original_path, edge_original_path)
        sample_edges = process_image(sample_path, edge_sample_path)

        is_original, score = compare_edges(original_edges, sample_edges)

        return jsonify({
            'message': '✅ The PCB is perfect' if is_original else '❌ The PCB is defective or has a missing component',
            'score': float(score),
            'result': 0 if is_original else 1
        })

    except Exception as e:
        print("Error in trigger_analysis:", str(e))
        return jsonify({'message': '❌ Server error during analysis.', 'result': -1}), 500

@app.route('/show_edges', methods=['POST'])
def show_edges():
    try:
        edge_original_path = os.path.join(EDGE_FOLDER, 'original_edge.jpg')
        edge_sample_path = os.path.join(EDGE_FOLDER, 'sample_edge.jpg')

        return jsonify({
            'original_edge_url': '/' + edge_original_path,
            'sample_edge_url': '/' + edge_sample_path
        })
    except Exception as e:
        print("Error in /show_edges:", str(e))
        return jsonify({'message': '❌ Could not load edge images.'}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
