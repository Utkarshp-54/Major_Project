from ultralytics import YOLO
import cv2
import cvzone
import math
import numpy as np

model = YOLO('yolov8n.pt')
classNames = ["person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat",
              "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
              "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella",
              "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball", "kite", "baseball bat",
              "baseball glove", "skateboard", "surfboard", "tennis racket", "bottle", "wine glass", "cup",
              "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange", "broccoli",
              "carrot", "hot dog", "pizza", "donut", "cake", "chair", "sofa", "pottedplant", "bed",
              "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard", "cell phone",
              "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors",
              "teddy bear", "hair drier", "toothbrush"
              ]
#cap = cv2.VideoCapture("http://192.168.218.102:8080/video")
#cap = cv2.VideoCapture("C:\\Users\\avikd\\Documents\\Python programming\\Untitled Folder\\Object-Detection-101\\Videos\\bikes.mp4")
cap = cv2.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

while True:
    success, img = cap.read()
    results = model(img, stream = True)
    for r in results:
        boxes = r.boxes
        for box in boxes:
            x1,y1,x2,y2 = box.xyxy[0]            
            x1,y1,x2,y2 = int(x1), int(y1),int (x2), int(y2)            
            
            w,h = x2-x1,y2-y1
            
            cvzone.cornerRect(img, (x1,y1,w,h))
            
            #Confidence
            conf = math.ceil((box.conf[0]*100))/100
            
            #class name
            cls = int(box.cls[0])
            cvzone.putTextRect(img,f'{classNames[cls]} {conf}',(max(0, x1),max(50,y1)))
            


    cv2.imshow("Image", img)
    if cv2.waitKey(1) == ord("q"):
        break;
        
cap.release()
cv2.destroyAllWindows()