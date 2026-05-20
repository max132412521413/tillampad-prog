import cv2
import mediapipe as mp
import serial
import time


#
#
## --LÅNAD KOD-- ##
#
#
arduino = serial.Serial('COM5', 9600)
time.sleep(2)

mp_hands = mp.solutions.hands
mp_draw = mp.solutions.drawing_utils

cap = cv2.VideoCapture(0)

prev_angle_x = 0
prev_angle_y = 0
alpha = 0.2  # smoothing

with mp_hands.Hands(min_detection_confidence=0.7,
                    min_tracking_confidence=0.7,
                    max_num_hands=1) as hands:

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        frame = cv2.flip(frame, 1)
        h, w, _ = frame.shape

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        result = hands.process(rgb)

        if result.multi_hand_landmarks:
            for hand_landmarks in result.multi_hand_landmarks:

                # Use wrist as reference point
                wrist = hand_landmarks.landmark[0]

                # Convert normalized → pixel space
                x = wrist.x
                y = wrist.y

                # Map to servo angles
                raw_angle_x = int(x * 180)        # left-right
                raw_angle_y = int((1 - y) * 180)  # up-down (invert)

                # Clamp
                raw_angle_x = max(0, min(180, raw_angle_x))
                raw_angle_y = max(0, min(180, raw_angle_y))

                # Smooth
                angle_x = int(alpha * raw_angle_x + (1 - alpha) * prev_angle_x)
                angle_y = int(alpha * raw_angle_y + (1 - alpha) * prev_angle_y)

                prev_angle_x = angle_x
                prev_angle_y = angle_y
#
#     ## --LÅNAD KOD SLUT-- ##
#

                # Laser on/off control
                fingertip = hand_landmarks.landmark[8]
                thumbtip = hand_landmarks.landmark[4]
                distance = ((fingertip.x-thumbtip.x)**2+(fingertip.y-thumbtip.y)**2)**0.5
                print(distance)
                # print(fingertip.x)


                # simple if-statment for the On/Off 
                if distance <= 0.035:
                    Laser = 1
                else: 
                    Laser = 0





                # Display
                cv2.putText(frame, f"X: {angle_x}", (30, 50),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
                cv2.putText(frame, f"Y: {angle_y}", (30, 100),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)

                # Send BOTH angles
                arduino.write(f"{angle_x},{angle_y}\n".encode())
                arduino.write(f"L:{Laser}\n".encode())

                mp_draw.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)



                
                
        # Turn Off key
        cv2.imshow("2 Servo Control", frame)

        if cv2.waitKey(1) & 0xFF == 27:  # ESC to quit
            break

cap.release()
#close arduino  function and terminate camra window function
arduino.close()
cv2.destroyAllWindows()