import keyboard
import serial
import time

SERIAL_PORT = 'COM6'  
BAUD_RATE = 115200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"Connected to ESP32 on {SERIAL_PORT}")
except Exception as e:
    print(f"Error: {e}")
    exit()

key_was_pressed = False

def record_activity(e):
    global key_was_pressed
    if e.name != 'esc': # Don't count the exit key as activity
        key_was_pressed = True

keyboard.on_press(record_activity)

print('Monitoring... Press "esc" to stop.')
last_send_time = time.time()

try:
    while True:
        if keyboard.is_pressed('esc'):
            break

        current_time = time.time()

        if current_time - last_send_time >= 1.0:
            if key_was_pressed:
                message = "key_pressed\n"
            else:
                message = "no_key_pressed\n"

            # Send to ESP32
            ser.write(message.encode('utf-8'))
            print(f"Interval Report: {message.strip()}")

            key_was_pressed = False
            last_send_time = current_time

        time.sleep(0.05)

finally:
    ser.close()
    print("Serial connection closed.")