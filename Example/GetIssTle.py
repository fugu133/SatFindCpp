import requests

url = 'https://celestrak.org/NORAD/elements/gp.php?CATNR=25544'
file_name = 'ISS.tle'
url_data = requests.get(url).content

with open(file_name, mode = 'wb') as f:
  f.write(url_data.replace(b'\r\n', b'\n'))
