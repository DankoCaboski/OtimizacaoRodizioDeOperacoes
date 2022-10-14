import PySimpleGUI as sg # Importação da lib PySimpleGUI como "sg"
sg.theme('DarkBlue13')

def primeira():
    Jan1=[
        [sg.Text('Bem Vindo', expand_x=True, justification='center', font=('Arial', 20))],
        [sg.Image(r'C:\Users\danko\Downloads\homem.png', size= (400,400))],
        [sg.Button('Seleção manual', expand_x=True, font=('Arial', 20), key='-MANUAL-'), sg.Button('Seleção manual', expand_x=True, font=('Arial', 20), key='-AUTO-')]
    ]
    return sg.Window('Controle de rodízio', layout=Jan1, margins=(10,10), finalize=True)
    
on = primeira()
sg.Window.Maximize(on)
while True:
    window, eventos, valores = sg.read_all_windows() 
    if eventos == sg.WINDOW_CLOSED: # Aqui estamos especificando o que será feito quando clicar no X da janela sendo "sg.WINDOW_CLOSE" o evento
        break # Este comando serve para encerrar uma repetição
   