import requests

URL = "http://127.0.0.1:8000"

r = requests.get(url=URL)

print(r.status_code)
print(r.text)