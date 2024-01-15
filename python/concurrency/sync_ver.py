import requests
import time

def download_site(sites):
    with requests.Session() as response:
        for url in sites:
            with response.get(url) as site:
                print(f"Read {len(site.content)} from {url}")

if __name__ == "__main__":
    sites = [
        "https://www.jython.org",
        "http://olympus.realpython.org/dice",
    ] * 20

    start_time = time.time()
    download_site(sites)
    end_time = time.time() - start_time
    print(f"Downloaded 80 sites in {end_time} seconds")
