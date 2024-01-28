import cv2
import tensorflow as tf
import numpy as np
from PIL import Image
import requests

# Load the TensorFlow model
model = tf.saved_model.load('./')
classes = ["cardboard", "glass", "metal", "paper", "plastic", "trash"]
frame_width = 640
frame_height = 480

# Region of interest
roi_width = int(frame_width * 2 / 3)    # 2/3 of the frame's width
roi_height = int(frame_height * 2 / 3)  # 2/3 of the frame's height
roi_x = 0
roi_y = frame_height - roi_height       # Start from 2/3 height from the top


# Start video capture from the second camera (index 1)
cap = cv2.VideoCapture(1)

# Check if the camera opened successfully
if not cap.isOpened():
    print("Error: Camera could not be opened.")
    cap.release()
    exit()

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: Failed to capture an image from the camera.")
        break

    cv2.rectangle(frame, (roi_x, roi_y), (roi_x + roi_width, roi_y + roi_height), (0, 255, 0), 2)
    
    # Display the frame
    cv2.imshow('Frame', frame)

    # Press 'c' to capture an image and process it
    if cv2.waitKey(1) & 0xFF == ord('c'):
        roi = frame[roi_y:roi_y+roi_height, roi_x:roi_x+roi_width]

        # Convert the captured frame to PIL Image
        img = Image.fromarray(roi)
        img = img.resize((300, 300 * img.size[1] // img.size[0]), Image.LANCZOS)
        
        # Save image
        img.save('test.jpg')
        inp_numpy = np.array(img)[None]

        # Prepare input for the model
        inp = tf.constant(inp_numpy, dtype='float32')

        # Get class scores from the model and find the prediction
        class_scores = model(inp)[0].numpy()
        prediction = classes[class_scores.argmax()]

        print("class_scores", class_scores)
        print("Class : ", prediction)

        # Construct the URL based on the prediction
        url = "http://192.168.55.75/"
        if prediction in ["cardboard", "glass", "paper", "plastic"]:
            url += "1"
        elif prediction == "metal":
            url += "2"
        elif prediction == "trash":
            url += "3"

        # Make a GET request to the web server
        response = requests.get(url)

        # Check if the request was successful and print the response
        if response.status_code == 200:
            print("Message sent successfully")
        else:
            print(f"Failed to send message. Status code: {response.status_code}")

    # Press 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close any OpenCV windows
cap.release()
cv2.destroyAllWindows()
