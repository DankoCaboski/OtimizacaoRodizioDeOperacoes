import random
import pandas  as pd
import time 

#                                                                                   7       8           9           10      11          12          
cabecalho1 = ['Registro','Nome','Area', 'Altura', 'Genero','Carga', 'Incapacidade', 'Over','TiranteE','TiranteD','Macaneta','Capota','Emblemas'] # Aqui estamos informando os nomes das colunas
Fixos = pd.DataFrame(data = pd.read_excel(r'C:\Users\danko\OneDrive\Documentos\GitHub\OtimizacaoRodizioDeOperacoes\python\VBA.XLSM', 'MEMBROS FIXOS'), columns=cabecalho1) # Aqui estamos criando um DataFrame com as informações do arquivo excel, mudar o endereco da pasta

cabecalho2 = ['Registro','Nome','Area', 'Altura', 'Genero','Carga', 'Incapacidade', 'Over','TiranteE','TiranteD','Macaneta','Capota','Emblemas'] # Aqui estamos informando os nomes das colunas
Estrangeiros = pd.DataFrame(data = pd.read_excel(r'C:\Users\danko\OneDrive\Documentos\GitHub\OtimizacaoRodizioDeOperacoes\python\VBA.XLSM', 'MEMBROS ESTRANGEIROS'), columns=cabecalho2) # Aqui estamos criando um DataFrame com as informações do arquivo excel, mudar o endereco da pasta

PessOver=[]#lista dos capacitados no overhead
PessTirE=[]#lista dos capacitados no tirante esquerdo
PessTirD=[]#lista dos capacitados no tirante direito
PessMaca=[]#lista dos capacitados na maçaneta
PessCapota=[]#lista dos capacitados na capota
PessEmblema=[]#lista dos capacitados nos emblemas

operacoes=[PessOver, PessTirE, PessTirD, PessMaca, PessCapota, PessEmblema]

def capacitados(): #função q gera lista dos capacitados na funcao desejada
    c=7
    p = 0       
    for ii in operacoes:
        l = 0 
        for ii in Fixos.itertuples():
            if Fixos.iat[l,c] == 'X':
                operacoes[p].append(Fixos.iat[l,1])
            l = l+1
        c = c+1
        p = p+1

oper = range(7,12)
selecionados=[]

def selecaoRandom():
    op = 0
    selecionados=[]
    for p in operacoes:
        if len(operacoes[op]) == 0:
            op+=1
            continue

        if len(operacoes[op]) == 1:
            selecionados.append(operacoes[op][0])
            print(f'operação {op} recebe {operacoes[op][0]}')
            selecionados.append(operacoes[op][0])
            for i in operacoes:
                if i == operacoes[op]:
                    continue
                elif person in operacoes[op] == True:                        
                    operacoes[op].remove(person)        
            print(f'operação {op} recebe {operacoes[op][pess]}')

        if len(operacoes[op]) == 2:
            pess = (random.randint(0,1))
            selecionados.append(operacoes[op][pess])
            for i in operacoes:
                if i == operacoes[op]:
                    continue
                elif person in operacoes[op] == True:                        
                    operacoes[op].remove(person)        
            print(f'operação {op} recebe {operacoes[op][pess]}')

        if len(operacoes[op])>2:
            pess = (random.randint(0,len(operacoes[op])-1))
            person = operacoes[op][pess]
            if person in selecionados:
                while True:
                    npess = random.randint(0,len(operacoes[op])-1)
                    person = operacoes[op][npess]
                    if person in selecionados:
                        continue
                    if person not in selecionados:
                        selecionados.append(person)
                        print(f'operação {op} recebe {person}')
                        break
            if person not in selecionados:
                selecionados.append(person) 
                print(f'operação {op} recebe {person}')   
                for i in operacoes:
                    if i == operacoes[op]:
                        continue
                    elif person in operacoes[op] == True:
                        operacoes[op].remove(person)     
        op +=1
        print (op)
        if op > 5:
            break

delay = 0.5 #setamos o tempo de delay, o temo de cada rodizio *2 deve ser igual ao contador do do tempo de rodizio
TempRod = int(input('informe em segundos o tempo de rodizio: '))#seta o tempo do rodizio
contTemp = 0    #contador de ciclos do programa

c = 0
while True:
    if c == 0:
        capacitados()
        selecaoRandom()
        c+=1
    if c >= 1:
        time.sleep(delay)#para a veredura do sistema e increment ao contador
        contTemp += 1 
        if contTemp > TempRod:
            break