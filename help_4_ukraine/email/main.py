import paho.mqtt.client as mqtt
import json
from flask import Flask
from flask_mail import Mail, Message

# Configure
broker_address="broker"
app = Flask(__name__)
app.app_context()

app.config['MAIL_SERVER']='smtp.mailtrap.io'
app.config['MAIL_PORT'] = 2525
app.config['MAIL_USERNAME'] = '803c0ea458f7f3'
app.config['MAIL_PASSWORD'] = '526a1c1d7fa910'
app.config['MAIL_USE_TLS'] = True
app.config['MAIL_USE_SSL'] = False
mail = Mail(app)

def send_email(payload):
    with app.app_context():
        msg = Message('Update for Help4Ukraine', sender = 'emailservice@help4ukraine.com', recipients = [payload['admin']])
        msg.body = f"""\
            Hi,

            User {payload['user']} has submitted a new donation for location {payload['location']}, of which you are an admin.

            {payload['donation']}

            Please get in contact with the user to finalise the donation details.
            Have a good day!

            This is an automatically generated email, do not reply
        """
        mail.send(msg)

# Callback for new message
def on_message(client, userdata, message):
    payload = str(message.payload.decode("utf-8"))
    print('[' + message.topic + '] received: ')

    try:
        print(json.loads(payload))
        send_email(json.loads(payload))
    except:
        print('couldn\'t decode')


client = mqtt.Client("Location 1")
client.on_message=on_message
print("connecting to broker")
client.connect(broker_address)
print("connected successfully, starting loop")
client.loop_start() #start the loop
client.subscribe("1")

while True:
    pass
