var btnSend = document.getElementById("btnSend")
var server1Field = document.getElementById("server1Field")
var server2Field = document.getElementById("server2Field")
var server3Field = document.getElementById("server3Field")
var timeZoneField = document.getElementById("timeZoneField")
var ssidField = document.getElementById("ssidField")
var passField = document.getElementById("passField")
var urlField = document.getElementById("urlField")
var portField = document.getElementById("portField")
var userField = document.getElementById("userField")
var passField = document.getElementById("passField")
var loader = document.getElementById("loader")
var fail = document.getElementById("fail")
var sucess = document.getElementById("sucess")
var subtitle = document.getElementById("subtitle")
var text = document.getElementById("text")
var mac = document.getElementById("mac")

function btnSend_onclick() {
    var provData = {
        ntpProv: {
            server1: server1Field.value,
            server2: server2Fieldserver1Field.value,
            server3: server3Fieldserver1Field.value,
            timeZone: timeZoneFieldserver1Field.value,
        },
        wifiProv: {
            ssid: ssidFieldserver1Field.value,
            pass: passFieldserver1Field.value,
        },
        mqttProv: {
            url: urlFieldserver1Field.value,
            port: portFieldserver1Field.value,
            user: userFieldserver1Field.value,
            pass: passFieldserver1Field.value,
        }
    }
    sendData(provData)
    showElement(loader, null, "Enviando credenciais...")
    btnSend.onclick = function() {}
}

function sendData(data) {
    var jsonStr = JSON.stringify(data)
    console.log(data)
    var xhr = new XMLHttpRequest()
    xhr.open("POST", "/?data=" + jsonStr, true)
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            btnSend.textContent = "Credenciais enviadas."
            interval = setInterval(() => {
                checkConnection()
            }, 1000)
        }
    }
    xhr.send()
}

function checkConnection() {
    var xhr = new XMLHttpRequest()
    xhr.onreadystatechange = function() {

        if (xhr.readyState == 4 && xhr.status == 200) {
            var data = xhr.responseText

            if (data == "PROV_WIFI") {
                btn.textContent = "Conectando..."
            }

            if (data == "WIFI_REFUSED") {
                clearInterval(interval)
                loader.style.display = "none"
                showElement(fail, "yellow", "Credenciais recusadas!")
                resetPage()
            }

            if (data == "PROV_NTP") {
                sucess.style.display = "none"
                showElement(loader, null, "Configurando Relógio...")
            }

            if (data == "NTP_ERR") {
                clearInterval(interval)
                loader.style.display = "none"
                showElement(fail, "yellow", "Erro ao configurar o relógio!")
                resetPage()
            }

            if (data == "PROV_MQTT") {
                sucess.style.display = "none"
                showElement(loader, null, "Conectando-se ao servidor mqtt...")
            }

            if (data == "PROV_MQTT_ERR") {
                clearInterval(interval)
                loader.style.display = "none"
                showElement(fail, "yellow", "Erro ao se conectar no servidor!")
                resetPage()
            }


            if (data.split(":").length == 6) {
                mac.textContent = data
                loader.style.display = "none"
                subtitle.textContent = "Concluir configuração"
                text.textContent = "Pressione continuar para concluir a configuração do dispositivo"
                mac.style.display = "block"
                btn.textContent = "Continuar"
                btn.onclick = function() {
                    sendData("C")
                    setTimeout(() => {
                        window.location.href = "home.htm"
                    }, 5000)
                }
            }

            if (data == "PROV_COMPLETE") {
                showElement(sucess, "green", "Configuração concluída!")
            }

        }
    }
    xhr.open("GET", "/?data=true", true)
    xhr.send()
}

function showElement(element, color, text) {
    element.style.display = "block"
    if (color != null) element.style.color = color
    btnSend.textContent = text
}

function resetPage() {
    setTimeout(() => {
        fail.style.display = "none"
        btnSend.textContent = "Enviar"
        btnSend.onclick = btnPress
    }, 2500)
}