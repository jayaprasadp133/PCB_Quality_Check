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

