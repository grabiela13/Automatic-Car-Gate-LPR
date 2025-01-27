import cv2
import easyocr
from validators import is_plate_valid, is_car_allowed


def get_signal():
    return True


def capture_image():
    # # Using IP camera
    # cap = cv2.VideoCapture('https://192.168.16.101:8080/video')

    # if not cap.isOpened():
    #     print("Error: Could not open the IP camera.")
    #     return None

    # # Capture a frame from the IP camera
    # ret, frame = cap.read()

    # if not ret:
    #     print("Error: Could not read frame from IP camera.")
    #     return None


    # # Capture video from the default webcam (index 0)
    cap = cv2.VideoCapture(0)  # 0 is the index for the default webcam

    if not cap.isOpened():
        print("Error: Could not open the webcam.")
        return None

    # Capture a frame from the webcam
    ret, frame = cap.read()

    if not ret:
        print("Error: Could not read frame from the webcam.")
        return None

    # Release the camera
    cap.release()

    # # Display the captured image (optional)
    # plt.imshow(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    # plt.title('Captured Image')
    # plt.show()

    return frame


def detect_license():
    img = capture_image()  # Capture image from webcam

    if img is None:
        return "Error in capturing image."

    # # Estimate the license plate position and crop image
    # # plate_position = (650, 500, 780, 270)  # Approximate coordinates (for IP camera)
    # plate_position = (250, 50, 200, 100) # For webcam
    # x, y, w, h = plate_position
    # img = img[y:y+h, x:x+w]

    # # Show cropped image
    # plt.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))
    # plt.title("Cropped Image")
    # plt.show()

    # Perform OCR on the cropped image
    reader = easyocr.Reader(['en'])
    result = reader.readtext(img)

    if result:
        print("OCR Detected Text:", result[0][-2])
        text = result[0][-2]
        return text

def main():
    while get_signal(): 
        plate = detect_license()

        if plate: 
            plate = plate.replace(" ", "")
            if not is_plate_valid(plate):
                pass # try until it is read correctly
            elif is_car_allowed(plate):
                print("Car granted access.")
                # return True
            else:
                print("Car denied access.")
                # return False

if __name__ == "__main__":
    main()
