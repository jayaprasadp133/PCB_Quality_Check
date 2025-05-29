# 🛠️ Quality Control of PCB Using Image Processing

This project automates the **defect detection** in Printed Circuit Boards (PCBs) using **Image Processing** techniques. It helps identify issues such as missing components, misaligned parts, short circuits, or soldering errors using a reference (ideal) PCB image.

---

## 📸 Project Overview

The system takes an image of a PCB and compares it with a reference image to identify and highlight defects. This approach is essential in electronics manufacturing for maintaining consistent quality standards without human intervention.

---

## ⚙️ Features

- 🔍 Image alignment and resizing
- 🧠 Image preprocessing (grayscale, blur, edge detection)
- 🔁 Image comparison using techniques like:
  - Absolute difference
  - Contour detection
  - Thresholding
- 📏 Defect region marking with bounding boxes
- 📊 Accuracy visualization

---

## 🧰 Technologies Used

| Tool        | Purpose                          |
|-------------|----------------------------------|
| Python      | Core programming language        |
| OpenCV      | Image processing and comparison  |
| NumPy       | Matrix operations and processing |
| Matplotlib  | Visualizing output (optional)    |

---

## 📁 Folder Structure

PCB_Quality_Check_final/
│
├── reference_images/ # Ideal PCB images
├── test_images/ # PCBs to check for defects
├── output_images/ # Processed output with defect highlights
├── utils/ # Custom utility functions (if any)
├── main.py # Main script to run the QC process
├── requirements.txt # Dependencies
└── README.md # Project documentation


---

## ▶️ How to Run

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/PCB_Quality_Check.git
cd PCB_Quality_Check


