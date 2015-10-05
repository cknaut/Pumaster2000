import scrapy
from pun.items import Pun

class punster(scrapy.Spider):
    name = "punster"
    allowed_domains = ["http://www.punoftheday.com/"]
    start_urls = []
    
    for year in range(1995,2015):
            for month in range(1,13):
                url1 = "http://www.punoftheday.com/cgi-bin/disppuns.pl?ord=M&cat=" + str(month) + "&sub=" + str(year) + "&page=1"
                url2 = "http://www.punoftheday.com/cgi-bin/disppuns.pl?ord=M&cat=" + str(month) + "&sub=" + str(year) + "&page=2"
                url3 = "http://www.punoftheday.com/cgi-bin/disppuns.pl?ord=M&cat=" + str(month) + "&sub=" + str(year) + "&page=3"
                start_urls.append(url1)
                start_urls.append(url2)
                start_urls.append(url3)
                

    def parse(self, response):
        table = table = response.xpath("/html/body/div/div[2]/table/tr")
        for entry in table:
            pun = entry.xpath(".//td/text()")
            pun = pun[1].extract()
            item = Pun()
            item["punchline"] = pun
            yield item
        

