const char PAGE_404notfound[] PROGMEM = R"=====(
<div id="content">
	<article>
		<h2>404 Ta stran ne obstaja</h2>
	</article>
</div>
)=====";

void handleNotFound() {
  String content = String(PAGE_head);
  if (is_authentified())content += String(PAGE_menu_logedin);
  else content += String(PAGE_menu_normal);
  content += String(PAGE_404notfound) + String(PAGE_foot);
  server.send(404, "text/html", content);
}
