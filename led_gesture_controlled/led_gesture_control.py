import cv2
import mediapipe as mp
import requests

ESP32_IP = "http://192.168.4.1"  # ESP32 default AP IP

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

cap = cv2.VideoCapture(0)

with mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7) as hands:
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            continue

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = hands.process(rgb)

        if results.multi_hand_landmarks:
            for hand in results.multi_hand_landmarks:
                mp_drawing.draw_landmarks(frame, hand, mp_hands.HAND_CONNECTIONS)

                # Simple gesture rule: open palm = P1, fist = P2
                landmarks = hand.landmark
                finger_open = landmarks[8].y < landmarks[6].y  # index finger open?

                if finger_open:  # Player 1
                    requests.get(f"{ESP32_IP}/p1led0")
                    print("Gesture → Player 1")
                else:  # Player 2
                    requests.get(f"{ESP32_IP}/p2led0")
                    print("Gesture → Player 2")

        cv2.imshow("Gesture Control", frame)
        if cv2.waitKey(5) & 0xFF == 27:  # ESC to exit
            break

cap.release()
cv2.destroyAllWindows()
