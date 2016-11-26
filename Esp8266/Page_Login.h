const char PAGE_login[] PROGMEM = R"=====(
		<form action="/login" method="POST">
			<table align="center">
				<tr>
					<td>
						<input type="text" name="USERNAME" placeholder="Uporabnisko ime">
					</td>
				</tr>
				<tr>
					<td>
						<input type="password" name="PASSWORD" placeholder="Geslo">
					</td>
				</tr>
				<tr>
					<td>
						<input class="shadow button" type="submit" value="Prijava">
					</td>
				</tr>
		  </table>
		</form>
	</div>
</div>
)=====";

void handleLogin() {
	if (server.hasHeader("Cookie")) {
		String cookie = server.header("Cookie");
	}
	if (server.hasArg("DISCONNECT")) {
		String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
		server.sendContent(header);
		return;
	}
	if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
		if (server.arg("USERNAME") == loginUsername &&  server.arg("PASSWORD") == loginPassword) {
			String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
			server.sendContent(header);
			return;
		}
		String msg = "Napačno uporabniško ime/geslo! Poizkusi znova.";
	}
	String content = String(PAGE_head);
	content += String(PAGE_menu_normal);
	content += String(PAGE_login);
	content += String(PAGE_foot);
	server.send(200, "text/html", content);
}

