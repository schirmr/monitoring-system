async function atualizar() {
    try {
        const resposta = await fetch("metricas.json?t=" + Date.now());
        
        const dados = await resposta.json();

        document.getElementById("mem-total").innerText = dados.memoria_total_gb.toFixed(2);
        document.getElementById("mem-usada").innerText = dados.memoria_usada_gb.toFixed(2);
        document.getElementById("mem-porcentagem").innerText = dados.porcentagem_memoria_usada.toFixed(1) + "%";

    } catch (erro) {
        console.log("Aguardando dados...");
    }
}

setInterval(atualizar, 1000);