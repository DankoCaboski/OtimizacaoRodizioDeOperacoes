import pandas  as pd
import time 

cabecalho1 = ['Registro','Nome','Area', 'Altura', 'Genero','Carga', 'Incapacidade' 'Over','TiranteE','TiranteD','Macaneta','Capota','Emblemas'] # Aqui estamos informando os nomes das colunas
Fixos = pd.DataFrame(data = pd.read_excel('VBA.XLSM', 'MEMBROS FIXOS'), columns=cabecalho1) # Aqui estamos criando um DataFrame com as informações do arquivo excel, mudar o endereco da pasta

cabecalho2 = ['Registro','Nome','Area', 'Altura', 'Genero','Carga', 'Incapacidade' 'Over','TiranteE','TiranteD','Macaneta','Capota','Emblemas'] # Aqui estamos informando os nomes das colunas
Estrangeiros = pd.DataFrame(data = pd.read_excel('VBA.XLSM', 'MEMBROS ESTRANGEIROS'), columns=cabecalho2) # Aqui estamos criando um DataFrame com as informações do arquivo excel, mudar o endereco da pasta

TempRod = int(input('informe em segundos o tempo de rodizio: '))
delay = 0.5 #setamos o tempo de delay, o temo de cada rodizio *2 deve ser igual ao contador do do tempo de rodizio 

PessOver=[]#lista dos capacitados no overhead
PessTirE=[]#lista dos capacitados no tirante esquerdo
PessTirD=[]#lista dos capacitados no tirante direito
PessMaça=[]#lista dos capacitados na maçaneta
PessCapota=[]#lista dos capacitados na capota
PessEmblema=[]#lista dos capacitados nos emblemas

operacoes=[PessOver, PessTirE, PessTirD, PessMaça, PessCapota, PessEmblema]

def capacitados(): #função q gera lista dos capacitados na funcao desejada
    x = 0 
    y = [6,7,8,9,10,11]#lista das operacoes
    for i in Fixos.itertuples():
        if Fixos.iat[x,l] == 'X':
            operacoes[2].append(Fixos.iat[x,1])
        x = x+1

contTemp = 0    #contador de ciclos do programa
while True:
    l = int(input('Informe l: '))
    capacitados()
    print(PessTirD)
    if TempRod > contTemp:
        time.sleep(delay)#para a veredura do sistema e increment ao contador 
        contTemp += 1
    break