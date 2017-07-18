const char PAGE_404notfound[] PROGMEM = R"=====(
		<h2>404 Ta stran ne obstaja</h2>
	</div>
</div>
)=====";

void handleNotFound() {
  String content = String(PAGE_head);
  if (is_authentified())content += String(PAGE_menu_logedin);
  else content += String(PAGE_menu_normal);
  content += String(PAGE_404notfound) + String(PAGE_foot);
  server.sendContent(content);
}
