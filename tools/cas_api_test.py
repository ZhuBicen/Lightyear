#!/usr/bin/python
import os.path
import httplib, urllib, urllib2, cookielib

# 1. Grab the Ticket Granting Ticket (TGT)

cas_host = "127.0.0.1:8443"
rest_endpoint = "/cas/v1/tickets"
params = urllib.urlencode({'username': 'admin2', 'password': 'admin2'})
#headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain", "User-Agent":"python" }
conn = httplib.HTTPSConnection(cas_host)
conn.request("POST", rest_endpoint, params)#, headers)
response = conn.getresponse()
print response.status, response.reason
data = response.read()
print "Data:", data
location = response.getheader('location')
#? Pull off the TGT from the end of the location, this works for CAS 3.3-FINAL
tgt = location[location.rfind('/') + 1:]
conn.close()

print "Location:", location
print "TGT:", tgt
print "***"

# # 2. Grab a service ticket (ST) for a CAS protected service

# document = '/secure/blah.txt'
# service = 'http://docs.acme.com%s' % (document)

# params = urllib.urlencode({'service': service })
# conn = httplib.HTTPSConnection(cas_host)
# conn.request("POST", "%s%s" % ( rest_endpoint, tgt ), params, headers)
# response = conn.getresponse()
# print response.status, response.reason
# st = response.read()
# conn.close()

# print "service: %s" % (service)
# print "st  : %s" % (st)
# print "***"

# # 3. Grab the protected document

# url = "%s?ticket=%s" % ( service, st ) # Use &ticket if service already has query parameters
# print "url??? : %s" % (url)

# cj = cookielib.CookieJar()

# # no proxies please
# no_proxy_support = urllib2.ProxyHandler({})
# # we need to handle session cookies AND redirects
# cookie_handler = urllib2.HTTPCookieProcessor(cj)

# opener = urllib2.build_opener(no_proxy_support, cookie_handler, urllib2.HTTPHandler(debuglevel=1))
# urllib2.install_opener(opener)
# protected_data = urllib2.urlopen(url).read()
# print protected_data
