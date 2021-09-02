var btn = document.getElementById("btn")
var ssidfield = document.getElementById("ssidfield")
var passfield = document.getElementById("passfield")
var server1field = document.getElementById("server1field")
var server2field = document.getElementById("server2field")
var server3field = document.getElementById("server3field")
var timezonefield = document.getElementById("timezonefield")
var burlfield = document.getElementById("urlfield")
var bportfield = document.getElementById("portfield")
var buserfield = document.getElementById("userfield")
var bpassfield = document.getElementById("passfield")
var loader = document.getElementById("loader")
var fail = document.getElementById("fail")
var sucess = document.getElementById("sucess")
var subtitle = document.getElementById("subtitle")
var text = document.getElementById("text")
var mac = document.getElementById("mac")

function btn_onclick() {
    var provData = {
        ntpProv: {
            server1: server1field.value,
            server2: server2field.value,
            server3: server3field.value,
            timeZone: timezonefield.value,
        },
        wifiProv: {
            ssid: ssidfield.value,
            pass: passfield.value,
        },
        mqttProv: {
            url: burlfield.value,
            port: bportfield.value,
            user: buserfield.value,
            pass: bpassfield.value,
        }
    }
    sendData(provData)
    showElement(loader, null, "Enviando credenciais...")
    btn.onclick = function() {}
}

function sendData(data) {
    var jsonStr = JSON.stringify(data)
    console.log(data)
    var xhr = new XMLHttpRequest()
    xhr.open("POST", "/?data=" + jsonStr, true)
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            btn.textContent = "Credenciais enviadas."
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
    btn.textContent = text
}

function resetPage() {
    setTimeout(() => {
        fail.style.display = "none"
        btn.textContent = "Enviar"
        btn.onclick = btn_onclick
    }, 2500)
}