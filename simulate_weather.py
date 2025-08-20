import requests
import random
import time

# 🔹 Replace with your ThingSpeak Write API Key
API_KEY = "YOUR_WRITE_API_KEY"

# ThingSpeak update URL
URL = "https://api.thingspeak.com/update"

while True:
    # Generate random temperature (20°C - 40°C) and humidity (40% - 90%)
    temperature = round(random.uniform(20, 40), 2)
    humidity = round(random.uniform(40, 90), 2)

    # Data payload
    payload = {
        "api_key": API_KEY,
        "field1": temperature,
        "field2": humidity
    }

    # Send request
    response = requests.get(URL, params=payload)

    if response.text == "0":
        print("⚠️ Data not uploaded (maybe API limit reached)")
    else:
        print(f"✅ Uploaded → Temp: {temperature}°C, Humidity: {humidity}%, Entry ID: {response.text}")

    # Wait 15 seconds (ThingSpeak free limit)
    time.sleep(15)
