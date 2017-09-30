#include <Update.h>

const char PAGE_update[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head></head>
<body>
<script type="text/javascript">
      function Redirect()
      {
        window.location="/";
      }
      setTimeout('Redirect()', 1000);
    </script>
</body>
</html>
)=====";

const char PAGE_noPermission[] PROGMEM = R"=====(
    <div id="content"> <article> <h1>You have no permission for this operation</h1>
    <script type="text/javascript">
    function Redirect()
    {
    window.location="/login";
    }
    setTimeout('Redirect()', 5000);
  </script>
)=====";

void handleSystemUpdate() {

  String content = String(PAGE_head);
  if (is_authentified())
  {
    content += String(PAGE_menu_logedin)+String(PAGE_update);
  }

  else{
    content += String(PAGE_menu_normal)+String(PAGE_noPermission);
  }
  content += String(PAGE_foot);
server.send(200, "text/html", content);
    //t_httpUpdate_return ret = ESPhttpUpdate.update("sshp.dejavu.si", 80, "/", releaseVersion);


}
