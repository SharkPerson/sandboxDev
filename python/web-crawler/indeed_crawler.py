import requests
from bs4 import BeautifulSoup

def scrape_indeed():

    web_url = "https://au.indeed.com/jobs?q=software+engineer&l=Australia"
    #web_url = "https://au.indeed.com/jobs?q=software+engineer&l=Australia&vjk=1d35646f214f6c96"
    
   # Define your custom headers here
    headers = {
    'authority': 'www.google.com',
    'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7',
    'accept-language': 'en-US,en;q=0.9',
    'cache-control': 'max-age=0',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36',
    # Add more headers as needed
    } 
    page = requests.get(web_url, headers=headers)
    
    if page.status_code == 200:
        print("Success")
    else:
        print("Failed")
    soup = BeautifulSoup(page.content, "html.parser")
    
    job_cards = soup.find_all("td", class_='resultContent')
    
    print(job_cards)
    
    for job_card in job_cards:
        print(job_card)

scrape_indeed()
