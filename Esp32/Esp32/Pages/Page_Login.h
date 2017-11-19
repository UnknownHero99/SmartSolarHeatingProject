const char PAGE_login[] PROGMEM = R"=====(
<div id="content">
  <article>
  <h1>Prijavi se</h1>
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
            <br />
            <input class="shadow button" type="submit" value="Prijava">
          </td>
        </tr>
      </table>
    </form>
  </article>
</div>

)=====";
