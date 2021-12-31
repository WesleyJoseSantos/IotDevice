var btn = document.getElementById("btn")
var ssid = document.getElementById("ssid")
var pass = document.getElementById("pass")
var server1 = document.getElementById("server1")
var server2 = document.getElementById("server2")
var server3 = document.getElementById("server3")
var timezone = document.getElementById("timezone")
var burl = document.getElementById("burl")
var bport = document.getElementById("bport")
var buser = document.getElementById("buser")
var bpass = document.getElementById("bpass")
var loader = document.getElementById("loader")
var fail = document.getElementById("fail")
var sucess = document.getElementById("sucess")
var subtitle = document.getElementById("subtitle")
var text = document.getElementById("text")
var mac = document.getElementById("mac")
var form = document.getElementById("form")

function btn_onclick() {
    var provData = {
        ntpProv: {
            server1: server1.value,
            server2: server2.value,
            server3: server3.value,
            timeZone: timezone.value,
        },
        wifiProv: {
            ssid: ssid.value,
            pass: pass.value,
        },
        mqttProv: {
            url: burl.value,
            port: bport.value,
            user: buser.value,
            pass: bpass.value,
        }
    }
    form.style.display = "none"
    showElement(loader, null, "Sending data...")
    sendData(provData)
    btn.onclick = function() {}
}

function sendData(data) {
    var jsonStr = JSON.stringify(data)
    console.log(jsonStr)
    var xhr = new XMLHttpRequest()
    xhr.open("POST", "/?data=" + jsonStr, true)
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = xhr.responseText
            if (data == "OK") {
                btn.textContent = "Data sended to device."
                interval = setInterval(() => {
                    checkConnection()
                }, 1000)
            }
        }
    }
    xhr.send()
}

function checkConnection() {
    var xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            var data = xhr.responseText
            console.log(data);

            if (data == "PROV_WIFI") {
                btn.textContent = "Connecting to wifi..."
            }

            if (data == "WIFI_REFUSED") {
                clearInterval(interval)
                loader.style.display = "none"
                showElement(fail, "yellow", "WiFi credentials error!")
                resetPage()
            }

            if (data == "PROV_NTP") {
                sucess.style.display = "none"
                showElement(loader, null, "NTP sync in progress...")
            }

            if (data == "NTP_ERR") {
                clearInterval(interval)
                loader.style.display = "none"
                showElement(fail, "yellow", "NTP sync error!")
                resetPage()
            }

            if (data == "PROV_MQTT") {
                sucess.style.display = "none"
                showElement(loader, null, "Connecting to MQTT...")
            }

            if (data == "PROV_MQTT_ERR") {
                clearInterval(interval)
                loader.style.display = "none"
                showElement(fail, "yellow", "MQTT connection error!")
                resetPage()
            }


            if (data.split(":").length == 6) {
                mac.textContent = data
                loader.style.display = "none"
                subtitle.textContent = "Confirm Configuration"
                text.textContent = "Press confirm button to confirm and finish device configuration proccess."
                mac.style.display = "block"
                btn.textContent = "Confirm"
                btn.onclick = function() {
                    sendData("C")
                    setTimeout(() => {
                        window.location.href = "home.htm"
                    }, 5000)
                }
            }

            if (data == "PROV_COMPLETE") {
                showElement(sucess, "green", "Configuration Complete!")
            }

        }
    }
    xhr.open("GET", "/?data=true", true)
    xhr.send()
}

function showElement(element, color, text) {
    element.style.display = "block"
    if (color != null) element.style.color = color
    btn.textContent = text
}

function resetPage() {
    setTimeout(() => {
        form.style.display = "block"
        fail.style.display = "none"
        btn.textContent = "Enviar"
        btn.onclick = btn_onclick
    }, 2500)
}