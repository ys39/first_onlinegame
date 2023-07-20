var fg_ctx = null;
var bg_ctx = null;
var ws = null;
var acc = null;
var hostname = "192.168.0.1";
var port = "8888";
var sendAccel = false;
var sw = window.screen.width;
var sh = window.screen.height;
var prev_ax = 0;
var prev_ay = 0;
var prev_az = 0;

function $(id) { return document.getElementById(id); }

function onDeviceReady() {
    navigator.splashscreen.hide();
    switchPage('app', 0);
}

function setting() {
    var div = document.createElement('div');
    div.className = "setting";
    var form = document.createElement('form');
    form.innerHTML = "<h1>接続先サーバ情報の入力画面</h1>";
    var table = document.createElement('table');
    for (var i = 0; i < 3; ++i) {
        var tr = table.insertRow();
        var th = document.createElement('th');
        th.className = "th";
        tr.appendChild(th);
        tr.insertCell();
    }
    table.rows[0].cells[0].innerText = "IP Address";
    table.rows[1].cells[0].innerText = "Port Number";
    table.rows[2].cells[0].innerText = "Accelerometer";

    var i = document.createElement('input');
    i.type = "text";
    i.id = "hostname";
    i.className = "input";
    i.value = hostname;
    table.rows[0].cells[1].appendChild(i);

    i = document.createElement('input');
    i.type = "text";
    i.id = "port";
    i.className = "input";
    i.value = port;
    table.rows[1].cells[1].appendChild(i);

    i = document.createElement('input');
    i.type = "checkbox";
    i.id = "sendAccel";
    i.className = "input";
    i.checked = sendAccel;
    table.rows[2].cells[1].appendChild(i);

    form.appendChild(table);
    div.appendChild(form);

    var button = document.createElement('button');
    button.className = "button";
    button.innerText = "接続開始！";
    button.setAttribute('ontouchstart', "switchPage('setting', 1);");
    div.appendChild(button);

    document.body.appendChild(div);
}

function connect() {
    var div = document.createElement('div');
    div.className = "connect";
    var fg = document.createElement('canvas');
    fg.id = "fg";
    div.appendChild(fg);
    var bg = document.createElement('canvas');
    bg.id = "bg";
    div.appendChild(bg);
    document.body.appendChild(div);
    WebSocketConnect();
    loadCanvas();
    loadContext();
}

function switchPage(from, to) {
    if (to == 0) {
        StatusBar.show();
        StatusBar.styleDefault();
        var elements = document.getElementsByClassName(from);
        while (elements.length > 0) {
            elements[0].parentNode.removeChild(elements[0]);
        }
        setting();
    } else {
        hostname = $("hostname").value;
        port = $("port").value;
        sendAccel = $("sendAccel").checked;
        var elements = document.getElementsByClassName(from);
        while (elements.length > 0) {
            elements[0].parentNode.removeChild(elements[0]);
        }
        StatusBar.hide();
        connect();
    }
}

function WebSocketConnect() {
    if ("WebSocket" in window) {
        ws = new WebSocket("ws://" + hostname + ":" + port + "/");
        ws.binaryType = "arraybuffer";
        ws.onopen = function () {
            var buffer = new Float32Array(3);
            buffer[0] = 0;
            buffer[1] = sw;
            buffer[2] = sh;
            ws.send(buffer);
            delete buffer;
            if (sendAccel) {
                accelStart();
            }
        };
        ws.onmessage = function (evt) {
            if (typeof evt.data == "string") {
                var received_msg = evt.data;
                navigator.notification.alert(received_msg, null, "遠隔タッチ", "OK");
            } else {
                var buffer = new Uint8Array(evt.data);
                if (buffer[0] == 0xff && buffer[1] == 0xd8) {
                    var imgType = "data:image/jpeg;base64,";
                } else if (buffer[0] == 0x89 && buffer[1] == 0x50) {
                    var imgType = "data:image/png;base64,";
                } else {
                    navigator.notification.alert("Unknown format", null, "遠隔タッチ", "OK");
                    var imgType = "";
                }
                if (imgType != "") {
                    var image = new Image();
                    image.width = sw;
                    image.height = sh;

                    var b64 = '';
                    for (var i = 0, len = buffer.byteLength; i < len; ++i) {
                        b64 += String.fromCharCode(buffer[i]);
                    }
                    image.src = imgType + window.btoa(b64);

                    image.onload = function () {
                        bg_ctx.drawImage(image, 0, 0);
                        delete image;
                    }
                }
            }
        };
        ws.onclose = function () {
            if (sendAccel) {
                accelStop();
            }
            if (ws != null) {
                ws.close();
                delete ws;
            }
            switchPage('connect', 0);
        };
        ws.onerror = function () {
            if (sendAccel) {
                accelStop();
            }
            navigator.notification.alert("サーバに接続できません", null, "遠隔タッチ", "OK");
            delete ws;
            switchPage('connect', 0);
        };
    }
    else {
        navigator.notification.alert("端末がWebSocketをサポートしていません", null, "遠隔タッチ", "OK");
        switchPage('connect', 0);
    }
}

function loadCanvas() {
    //ロード時にcanvasのサイズをデバイスのサイズにする
    $("fg").width = sw;
    $("fg").height = sh;
    //"ontouch~"イベントはマルチタッチに対応している
    $("fg").ontouchstart = onTouchStart;
    $("fg").ontouchmove = onTouchMove;
    $("fg").ontouchend = onTouchEnd;

    $("bg").width = sw;
    $("bg").height = sh;
}
function loadContext() {
    fg_ctx = $("fg").getContext("2d");
    fg_ctx.lineWidth = 1;
    fg_ctx.strokeStyle = "rgba(0, 192, 0, 1)";
    fg_ctx.fillStyle = "rgba(255, 255, 0, 0.5)";
    bg_ctx = $("bg").getContext("2d");
}

function onTouchStart(event) {
    event.preventDefault();
    var point = new Float32Array(event.touches.length * 2 + 1);
    eraseMark();
    point[0] = 1;
    for (var i = 0; i < event.touches.length; i++) {
        var t = event.touches[i];
        point[i * 2 + 1] = t.pageX;
        point[i * 2 + 2] = t.pageY;
        drawMark(t.pageX, t.pageY);
    }
    ws.send(point);
    delete point;
    //    navigator.accelerometer.getCurrentAcceleration(onAcc, null);
}

function onTouchEnd(event) {
    event.preventDefault();
    var point = new Float32Array(event.touches.length * 2 + 1);
    eraseMark();
    point[0] = 3;
    for (var i = 0; i < event.touches.length; i++) {
        var t = event.touches[i];
        point[i * 2 + 1] = t.pageX;
        point[i * 2 + 2] = t.pageY;
        drawMark(t.pageX, t.pageY);
    }
    ws.send(point);
    delete point;
}

function onTouchMove(event) {
    event.preventDefault();
    var point = new Float32Array(event.touches.length * 2 + 1);
    eraseMark();
    point[0] = 2;
    for (var i = 0; i < event.touches.length; i++) {
        var t = event.touches[i];
        point[i * 2 + 1] = t.pageX;
        point[i * 2 + 2] = t.pageY;
        drawMark(t.pageX, t.pageY);
    }
    ws.send(point);
    delete point;
}

function eraseMark() {
    fg_ctx.clearRect(0, 0, sw, sh);
}

function drawMark(mx, my) {
    fg_ctx.beginPath();
    fg_ctx.arc(mx, my, 25, 0, 360, false);
    fg_ctx.fill();
    fg_ctx.stroke();
}

function accelStart() {
    acc = navigator.accelerometer.watchAcceleration(onAccel, null, { frequency: 50 });
}

function onAccel(acceleration) {
    if (((acceleration.x - prev_ax) * (acceleration.x - prev_ax)
	+ (acceleration.y - prev_ay) * (acceleration.y - prev_ay)
	+ (acceleration.z - prev_az) * (acceleration.y - prev_ay)) > 0.01) {
        var data = new Float32Array(4);
        data[0] = 4;
        data[1] = acceleration.x;
        data[2] = acceleration.y;
        data[3] = acceleration.z;
        prev_ax = acceleration.x;
        prev_ay = acceleration.y;
        prev_az = acceleration.z;
        ws.send(data);
        delete data;
    }
}

function accelStop() {
    navigator.accelerometer.clearWatch(acc);
}

document.addEventListener('deviceready', onDeviceReady, false);
