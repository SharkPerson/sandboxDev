import requests
from bs4 import BeautifulSoup


# This only works for static websites since it gets the HTML code. Dynamic websites send JavaScript code that needs to be run.BeautifulSoup
# There is another library that can do this. requests-html, also something like selenium can work.
URL = "https://realpython.github.io/fake-jobs/"
r = requests.get(URL)

#print(r.text)

# .content holds teh raw bytes which can be decoded better than the .text
soup = BeautifulSoup(r.content, "html.parser")
#print(r.content)

results = soup.find(id="ResultsContainer")
#print(results.prettify())


# class is a reserved word, need to use class_
job_elements = soup.find_all("div", class_='card-content')

for job_element in job_elements: 
    job_title = job_element.find("h2", class_='title is-5')
    company_name = job_element.find("h3", class_='subtitle is-6 company')
    location_element = job_element.find("p", class_='location')
    print(job_title.text.strip())
    print(company_name.text.strip()) 
    print(location_element.text.strip())
    print()

python_jobs = soup.find_all("h2", string=lambda text: "python" in text.lower())
print(len(python_jobs))

python_job_elements = [h2_element.parent.parent.parent for h2_element in python_jobs]

for job_element in python_job_elements:
    links = job_element.find_all("a")[1]
    for link in links:
        link_url = link["href"]
        print(link_url)

