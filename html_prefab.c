/* This is a prepackaged response for 404 requests. */
const char html_404_page[] = "HTTP/1.0 404 Page Not Found\r\n"
   "Server: ekans\r\n"
   "Content-Type: text/html\r\n"
   "\r\n"
   "<!doctype html>\n"
   "<html>\n"
   "  <body>\n"
   "    <h1>404 -- Page Not Found</h1>\n"
   "    <p>\n"
   "      The page you requested could not be located or does not exist.\n"
   "    </p>\n"
   "  </body>\n"
   "</html>\n";

const int html_404_length = sizeof html_404_page;

/* Error: Not Implemented. This response gets sent to clients making requests *
 * which are either unsupported HTTP/1.1 methods or just plain malformed.     */
const char html_501_page[] = "HTTP/1.0 501 Request Method Not Implemented\r\n"
   "Server: ekans\r\n"
   "Content-Type: text/html\r\n"
   "\r\n"
   "<!doctype html>\n"
   "<html>\n"
   "  <body>\n"
   "    <h1>501 -- Request Method Not Implemented</h1>\n"
   "    <p>\n"
   "      Your client (probably a web browser) asked us to do something we\n"
   "      don't know how to do. It was likely either an HTTP/1.1 request (the\n"
   "      ekans webserver doesn't need to support anything above HTTP/1.0 to\n"
   "      work properly), or a purely malformed request. Chances are you were\n"
   "      doing something weird, stupid, or malicious when this happened.\n"
   "    </p>\n"
   /* But but but... I had too... It's so annoying... */
   "    <p>\n"
   "      If you are using Microsoft's insecure, unstable, slow, poorly\n"
   "      written, standards noncompliant \"<strong>Internet Explorer\n"
   "      </strong>\" application in a fruitless attempt to browse the web,\n"
   "      visit <a href=\"http://www.google.com/chrome\">this website</a>\n"
   "      for a refreshing improvement.\n"
   "    </p>\n"
   "  </body>\n"
   "</html>\n";

const int html_501_length = sizeof html_501_page;
