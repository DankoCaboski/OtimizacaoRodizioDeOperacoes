import random
import pandas  as pd
import time
import  serial 

porta = serial.Serial('COM3', 9600, timeout = 1)

path = str(r'C:\Users\danko\OneDrive\Documentos\GitHub\OtimizacaoRodizioDeOperacoes\python\VBA.XLSM')
#                                                                                   7       8           9           10      11          12          
cabecalho1 = ['Registro','HEX','Nome','Area', 'Altura', 'Genero','Carga', 'Incapacidade', 'Over','TiranteE','TiranteD','Macaneta','Capota','Emblemas'] # Aqui estamos informando os nomes das colunas
Fixos = pd.DataFrame(data = pd.read_excel(path, 'MEMBROS FIXOS'), columns=cabecalho1) # Aqui estamos criando um DataFrame com as informações do arquivo excel, mudar o endereco da pasta

cabecalho2 = ['Registro','Nome','Area', 'Altura', 'Genero','Carga', 'Incapacidade', 'Over','TiranteE','TiranteD','Macaneta','Capota','Emblemas'] # Aqui estamos informando os nomes das colunas
Estrangeiros = pd.DataFrame(data = pd.read_excel(path, 'MEMBROS ESTRANGEIROS'), columns=cabecalho2) # Aqui estamos criando um DataFrame com as informações do arquivo excel, mudar o endereco da pasta

PessOver=[]#lista dos capacitados no overhead
PessTirE=[]#lista dos capacitados no tirante esquerdo
PessTirD=[]#lista dos capacitados no tirante direito
PessMaca=[]#lista dos capacitados na maçaneta
PessCapota=[]#lista dos capacitados na capota
PessEmblema=[]#lista dos capacitados nos emblemas

operacoes=[PessOver, PessTirE, PessTirD, PessMaca, PessCapota, PessEmblema]

def capacitados(): #função q gera lista dos capacitados na funcao desejada
    c=8
    p = 0       
    for i in operacoes:
        l = 0 
        for ii in Fixos.itertuples():
            if Fixos.iat[l,c] == 'X':
                operacoes[p].append(Fixos.iat[l,2])
            l = l+1
        c = c+1
        p = p+1

selecionados=[]

def selecaoRandom():
    op = 0
    for p in operacoes:
        if len(operacoes[op]) == 0:
            op+=1
#            print(f'sem pessoas capacitadas para a operação {operacoes[op]}')
            continue

        if len(operacoes[op]) == 1:
            person = operacoes[op][0]
            selecionados.append(person)
#            print(f'operação {op} recebe {operacoes[op][0]}')
            selecionados.append(operacoes[op][0])
            for i in operacoes:
                if i == operacoes[op]:
                    continue
                elif person in operacoes[op] == True:                        
                    operacoes[op].remove(person)        
#            print(f'operação {op} recebe {operacoes[op][pess]}')

        if len(operacoes[op]) == 2:
            person = operacoes[op][(random.randint(0,1))]
            selecionados.append(person)
            for i in operacoes:
                if i == operacoes[op]:
                    continue
                if person in operacoes[op]:                        
                    operacoes[op].remove(person)        
#            print(f'operação {op} recebe {operacoes[op][pess]}')

        if len(operacoes[op])>2:
            person = operacoes[op][random.randint(0,len(operacoes[op])-1)]
            if person in selecionados:
                while True:
                    person = operacoes[op][random.randint(0,len(operacoes[op])-1)] 
                    if person not in selecionados:
                        break
            selecionados.append(person) 
#                print(f'operação {op} recebe {person}')   
            for i in operacoes:
                if i == operacoes[op]:
                    continue
                if person in operacoes[op]:
                    operacoes[op].remove(person)     
        op +=1
#        print (op)
        if op > 5:
            break


delay = 0.5 #setamos o tempo de delay, o temo de cada rodizio *2 deve ser igual ao contador do do tempo de rodizio
Nrodizio = int(input('informe o  nuero de rodízios: '))
TempRod = int(input('informe em segundos o tempo de rodizio: '))#seta o tempo do rodizio
contTemp = 0    #contador de ciclos do programa

c = 0
while True:
    if c == 0:
        capacitados()
        selecaoRandom()
        print(selecionados)
        c+=1
        contTemp += 1 
        #Fixos.to_excel(writer,index=False,header=False,startrow=len(reader)+1)

    if c >= 1:
        time.sleep(delay)#para a veredura do sistema e increment ao contador
        contTemp += 1 
        selecionados.clear()
        selecaoRandom()
        capacitados()
        print(selecionados)
    if contTemp == Nrodizio:
        break
          