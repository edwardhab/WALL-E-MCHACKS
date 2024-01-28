# WALL-E Project - README

![WALL-E Robot](https://i.imgur.com/QZ2CDYI.jpeg)

## Our Dataset
For this project, we utilized an open-source dataset available on [Kaggle](https://www.kaggle.com/datasets/asdasdasasdas/garbage-classification), which classifies trash into six distinct categories. To enhance the model's accuracy in our specific setting, we modified the dataset by adding images that reflect the background and environment where WALL-E operates. This customization was crucial in improving the model's performance and ensuring it could accurately recognize waste in the robot's actual setting.

## Model Training
The core of our machine learning approach involved training a classifier model with the modified dataset. We used Liner, an online platform that facilitates model training, allowing us to fine-tune hyperparameters and adjust our dataset on the go. Although we experimented with alternative models, including an object-detection model based on Yolov8, our custom-trained classifier yielded the best results. This model proved to be highly effective in recognizing and categorizing various types of waste accurately.

## Hardware and Construction
The construction and assembly of the WALL-E robot was a collaborative effort. Our team was divided into two groups: one focused on the robotics aspect, working with Arduino and servo motors, and the other dedicated to building the robot's frame and external components using cardboard. This division of labor allowed for efficient progress in both the technical and structural elements of the project.

## Final State of WALL-E
The final setup of WALL-E involves an Arduino with a wifi module hosting a web server. This server is designed to handle client requests, integrating seamlessly with our machine learning model. 

When an item is presented to WALL-E, the model analyzes the image from the robot's camera and determines the item's category. Based on this classification, a request is sent to the Arduino web server, which then activates the servo motors to present the correct compartment for the user to dispose of their item. This system ensures a smooth and automated process, from waste recognition to appropriate disposal.
