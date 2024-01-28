import cv2
import requests
import tensorflow as tf
import numpy as np
from PIL import Image

# Load the TensorFlow model
model = tf.saved_model.load('./')
classes = ["paper", "metal", "cardboard", "trash", "glass", "plastic", "not trash"]

# Create a VideoCapture object to capture video from the camera
cap = cv2.VideoCapture(0)

counter = 0
last_prediction = None
last_found = None
while True:
    # Read the frame from the camera
    ret, frame = cap.read()

    # Convert the frame to RGB format
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Resize the frame to match the input size of the model
    img = Image.fromarray(rgb_frame)
    img = img.resize((300, 300 * img.size[1] // img.size[0]))

    # Convert the image to a NumPy array
    inp_numpy = np.array(img)[None]

    # Create a TensorFlow constant from the input array
    inp = tf.constant(inp_numpy, dtype='float32')

    # Get the class scores from the model
    class_scores = model(inp)[0].numpy()

    # Get the predicted class label
    predicted_class = classes[class_scores.argmax()]

    # Display the frame and predicted class label
    cv2.putText(frame, f"Class: {predicted_class}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)
    cv2.imshow('Camera', frame)
    if last_prediction == predicted_class:
        counter += 1
    else:
        counter = 0
    if counter == 20 and last_found != predicted_class:
        last_found = predicted_class
        print(f"Found {predicted_class}")
        if not predicted_class == "not trash":
            # send request to arduino:
            # Construct the URL based on the prediction
            url = "http://192.168.55.75/"
            if predicted_class in ["cardboard", "glass", "paper", "plastic"]:
                url += "1"
            elif predicted_class == "metal":
                url += "2"
            elif predicted_class == "trash":
                url += "3"
            
            # Make a GET request to the web server
            response = requests.get(url)
            # Check if the request was successful and print the response
            if response.status_code == 200:
                print("Message sent successfully")
            else:
                print(f"Failed to send message. Status code: {response.status_code}")

    last_prediction = predicted_class
    # Break the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the VideoCapture object and close the windows
cap.release()
cv2.destroyAllWindows()