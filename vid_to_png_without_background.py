import cv2
import numpy as np
from PIL import Image
import os

def remove_green_screen(input_video, output_folder):
    os.makedirs(output_folder, exist_ok=True)

    # Open the video file
    video = cv2.VideoCapture(input_video)

    frame_count = 0

    while True:
        # Read a frame from the video
        success, frame = video.read()

        if not success:
            break

        # Convert the frame from BGR to RGB
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        # Create a binary mask of the green screen
        lower_green = np.array([0, 100, 0])
        upper_green = np.array([100, 255, 100])
        mask = cv2.inRange(frame_rgb, lower_green, upper_green)

        # Invert the mask
        mask_inv = cv2.bitwise_not(mask)

        # Convert the frame to a PIL Image
        pil_image = Image.fromarray(frame_rgb)

        # Create a new image with an alpha channel
        rgba_image = pil_image.convert('RGBA')

        # Get the image data as a numpy array
        data = np.array(rgba_image)

        # Set alpha to 0 wherever the mask is 0 (green screen area)
        data[:, :, 3] = mask_inv

        # Create a new image from the modified data
        result = Image.fromarray(data)

        # Save the result as a PNG file
        output_file = os.path.join(output_folder, f'{frame_count}.png')
        result.save(output_file)

        frame_count += 1

    # Release the video capture object
    video.release()

    print(f"Processed {frame_count} frames. Output saved in {output_folder}")

input_video = './Textures/png/MW2 M4 Inspect and Reload Green screen.mp4'
output_folder = './guns'
remove_green_screen(input_video, output_folder)

# import cv2
# import numpy as np
# from PIL import Image
# import os

# def remove_green_screen(input_video, output_folder, skip_frames=5):
#     os.makedirs(output_folder, exist_ok=True)

#     # Open the video file
#     video = cv2.VideoCapture(input_video)

#     frame_count = 0

#     while True:
#         # Read a frame from the video
#         success, frame = video.read()

#         if not success:
#             break

#         # Skip frames
#         for _ in range(skip_frames):
#             success, frame = video.read()
#             if not success:
#                 break

#         if not success:
#             break

#         # Convert the frame from BGR to RGB
#         frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

#         # Create a binary mask of the green screen
#         lower_green = np.array([0, 100, 0])
#         upper_green = np.array([100, 255, 100])
#         mask = cv2.inRange(frame_rgb, lower_green, upper_green)

#         # Invert the mask
#         mask_inv = cv2.bitwise_not(mask)

#         # Convert the frame to a PIL Image
#         pil_image = Image.fromarray(frame_rgb)

#         # Create a new image with an alpha channel
#         rgba_image = pil_image.convert('RGBA')

#         # Get the image data as a numpy array
#         data = np.array(rgba_image)

#         # Set alpha to 0 wherever the mask is 0 (green screen area)
#         data[:, :, 3] = mask_inv

#         # Create a new image from the modified data
#         result = Image.fromarray(data)

#         # Save the result as a PNG file
#         output_file = os.path.join(output_folder, f'{frame_count}.png')
#         result.save(output_file)

#         frame_count += 1

#     # Release the video capture object
#     video.release()

#     print(f"Processed {frame_count} frames. Output saved in {output_folder}")

# input_video = './Textures/png/MW2 M4 Inspect and Reload Green screen.mp4'
# output_folder = './Guns_2'
# remove_green_screen(input_video, output_folder)
