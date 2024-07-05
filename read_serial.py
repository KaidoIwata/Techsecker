import serial
import requests
from datetime import datetime
import pytz

# シリアルポートの設定
ser = serial.Serial('/dev/tty.usbserial-◯◯◯', 115200, timeout=1)

# サーバーのURL
url = "サーバーURL"

# HTTPヘッダーを設定する
headers = {
    "Content-Type": "application/json"
}

def send_data_to_server(heart_rate):
    # 現在時刻をUTCで取得
    utc_now = datetime.now(pytz.utc).isoformat()
    
    # JSON配列としてデータを送信
    data = [
        {
            "timestamp": utc_now,
            "field": "heartrate",
            "value": float(heart_rate)
        }
    ]
    
    # POSTリクエストを送信
    response = requests.post(url, json=data, headers=headers)
    
    # サーバーからのレスポンスを表示
    print(f"Status Code: {response.status_code}")
    print(f"Response Body: {response.text}")

def read_from_serial():
    try:
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                heart_rate = float(line.replace('bpm', ''))
                print("Received from serial:", line)
                send_data_to_server(heart_rate)
    except serial.SerialException:
        print("シリアルデバイスが切断されました。")
        ser.close()

if __name__ == '__main__':
    try:
        print("シリアルポートからの読み取りを開始します...")
        read_from_serial()
    except KeyboardInterrupt:
        print("プログラムを終了します...")
    finally:
        ser.close()
