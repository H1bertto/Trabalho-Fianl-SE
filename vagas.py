from datetime import datetime


# Lê e processa os dados da porta Serial
def arduino_read(arduino, cout_max):
    vagas = {}
    vagas_bin = []
    while not vagas_bin \
            or b'-' not in vagas_bin[0]:
        vagas_bin = arduino.readlines()
    count = 0
    for vaga in vagas_bin:
        vaga = vaga.decode('ascii').split('-')
        if len(vaga) < 2:
            break
        vagas[vaga[0]] = vaga[1].strip()
        if count == cout_max:
            break
    return vagas


# Checa as vagas, passando a quantidade máxima
# de vagas no estacionamento
def get_vagas(arduino):
    value = arduino_read(arduino, 3)
    return value


# Processa template de vagas
def vagas_table_template(result):
    table = f"""
            <meta http-equiv="refresh" content="5" />
            <table class style="border: 1px solid black">
                <tr>
                    <th class style="text-align: center; padding: 15px; font-size: 30px;" colspan="2">
                        Vagas                
                    </th>
                </tr>
            """
    count_disp = 0
    count_ocup = 0
    count_esp = 0
    for vaga in result:
        color = "#45f75a"
        if vaga == "V1" and result[vaga] != "Ocupado":
            color = "#4580f7"
            count_esp += 1
        elif result[vaga] == "Ocupado":
            color = "#f74545"
            count_ocup += 1
        else:
            count_disp += 1
        table += f"""
                    <tr class style="border: 1px solid black">
                        <td class style="border: 1px solid black; padding: 10px;">
                            {vaga}
                        </td>
                        <td class style="border: 1px solid black; padding: 10px; background-color: {color}">
                            {result[vaga]}
                        </td>
                    </tr>"""
    table += f"""</table>
                    <br>
                    <p>Total de Vagas: {len(result)}</p>
                    <p>Total de Vagas Disponiveis (Todas): {count_disp + count_esp}</p>
                    <p>Total de Vagas Normais Disponiveis: {count_disp}</p>
                    <p>Total de Vagas Especiais Disponiveis: {count_esp}</p>
                    <p>Total de Vagas Ocupadas: {count_ocup}</p>
                    <br>
                    Última Atualização em: {datetime.strftime(datetime.now(), "%d %B %Y %X")}"""
    return table
