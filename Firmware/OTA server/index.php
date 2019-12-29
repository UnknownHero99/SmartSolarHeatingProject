<?PHP

header('Content-type: text/plain; charset=utf8', true);

function check_header($name, $value = false) {
    if(!isset($_SERVER[$name])) {
        return false;
    }
    if($value && $_SERVER[$name] != $value) {
        return false;
    }
    return true;
}

function getVersions($firmwareVersionsPath = 'firmware'){
	$dirs = glob($firmwareVersionsPath . '/*', GLOB_ONLYDIR);
	return $dirs;
}

function getLatestVersionPath(){
	$versions = getVersions();
	$latestVersionPath = end($versions);
	return $latestVersionPath;
}

function getLatestVersion(){
	return explode("/",getLatestVersionPath())[1];
}


function sendLatestVersion(){

	$latestVersion = "1.1";
	header($_SERVER["SERVER_PROTOCOL"]. ' 200 ' . $latestVersion, true, 403);
		echo "latestVersion is " . $latestVersion;
	
}

sendLatestVersion();

