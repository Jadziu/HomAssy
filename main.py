import paho.mqtt.client as mqtt
import config
import time
import multiprocessing

con1, con2 = multiprocessing.Pipe()
q = multiprocessing.Queue()
h = multiprocessing.Queue()


def mqtt_handler(q, h):
    mqtt_username = config.mqtt_username
    mqtt_password = config.mqtt_password
    mqtt_topic = config.mqtt_topic
    mqtt_broker_ip = config.mqtt_broker_ip
    client = mqtt.Client()
    client.username_pw_set(mqtt_username, mqtt_password)

    try:
        client.connect(mqtt_broker_ip, 1883)
        connection = True
        print('Connected to broker')
    except:
        print('Broker not responding!!!')
        connection = False

    def on_connect(client, userdata, flags, rc):
        print("Connected!", str(rc))
        client.subscribe(mqtt_topic)

    def on_message(client, userdata, msg):
        timestamp = time.strftime("%H:%M:%S")
        data = f"{timestamp}'{msg.topic}'{str(msg.payload)}"
        q.put(data)
        publish(client)
        # print(data)

    def publish(client):
        # ONLY FOR TEST
        holiday_data = h.get()
        if mqtt_topic == '+/+':
            client.publish('holiday', holiday_data)

    client.on_connect = on_connect
    client.on_message = on_message
    client.loop_forever()
    client.disconnect()


def data_handler(q):
    print('alive')
    while True:
        msg = q.get()
        msg2 = msg.split("'")
        print(msg2)
        print(msg2[0])
        th = msg2[3].split(",")
        print(th[0])
        print(th[1].strip())


def holiday_control(h):
    while True:
        holiday = 0
        h.put(holiday)


if __name__ == "__main__":
    p1 = multiprocessing.Process(target=mqtt_handler, args=(q, h,))
    p2 = multiprocessing.Process(target=data_handler, args=(q,))
    p3 = multiprocessing.Process(target=holiday_control, args=(h,))
    p1.start()
    p2.start()
    p3.start()
    p1.join()
    p2.join()
    p3.join()

