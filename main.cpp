#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;

// Estruturas
typedef struct
{
    int codigo;
    char marca[20];
    char Modelo[20];
    char placa[10];
    char categoria; // B-básico, I-intermediário, S-super
    bool disponivel;
    int quantLocacoes;
} Veiculo;

typedef struct
{
    char dataLocacao[11]; // Formato: DD/MM/AAAA
    char cpfCliente[12];
    int codigoVeiculo;
    int quantDias;
    bool ativa;
} Locacao;

// Declaração das funções
void cadastrarVeiculo();
void locarVeiculo();
bool validarCPF(string&);
bool validarData(string, int&, int&, int&);
void devolucaoVeiculo();
void relatorio();
void VeiculoDisponivel(int);
void atualizaLocacoes(string&);

int main()
{
    // Arquivo para armazenar locações (modo de acréscimo)
    ofstream arquivoLocados("LOCADOS.TXT", ios::out | ios::app);

    if(!arquivoLocados){
        cout << "Ocorreu algum erro com o arquivo!"; exit(0);}

    // Variável para armazenar a escolha do usuário
    int resp;

    // Loop do menu principal
    do
    {
        // Exibe o menu
        cout << "========MENU========" << endl;
        cout << "1. Cadastrar Veiculo" << endl;
        cout << "2. Realizar locacao" << endl;
        cout << "3. Realizar devolucao" << endl;
        cout << "4. Relatório" << endl;
        cout << "5. Sair" << endl;

        // Lê a escolha do usuário
        cin >> resp;
        cin.ignore(); // Limpa o caractere de nova linha

        // Switch para as opções do menu
        switch(resp)
        {
        case 1:
            cadastrarVeiculo();
            break;
        case 2:
            locarVeiculo();
            break;
        case 3:
            devolucaoVeiculo();
            break;
        case 4:
            relatorio();
            break;
        case 5:
            cout << "===============Encerrando o programa!=================" << endl;
            break;
        default:
            cout << "*DIGITE UMA OPCAO VALIDA***" << endl;
            break;

        }

    }
    while (resp != 5);   // Loop continua até que o usuário escolha sair

    // Fecha o arquivo de locações
    arquivoLocados.close();

    // Retorna 0 indicando término bem-sucedido
    return 0;
}

// Função para cadastrar um veículo
void cadastrarVeiculo()
{
    // Abre um arquivo de saída para escrever os dados da frota (modo binário)
    ofstream arquivoFrota("FROTA.DAD", ios::out | ios::binary | ios :: app);

    // Abre um arquivo de leitura para verificar se o veículo já existe na frota (modo binário)
    ifstream arquivoLeitura("FROTA.DAD", ios::binary);

    if(!arquivoFrota or !arquivoFrota){
        cout << "Ocorreu algum erro com algum ou com os dois arquivos!"; exit(0);}

    // Declaração de uma variável do tipo Veiculo para armazenar os dados do veículo
    Veiculo criado;

    // Strings para armazenar modelo, marca e placa do veículo
    string modelo, marca, placa = "";

    // Variável para armazenar a categoria do veículo
    char categoria;

    // Loop para ler cada veículo no arquivo e verificar se a placa já existe
    while (arquivoLeitura.read((char*)(&criado), sizeof(Veiculo)))
    {
        if (criado.placa == placa.c_str());
        {
            // Se a placa já existe, exibe uma mensagem e sai da função
            cout << "Veículo com a placa '" << placa << "' já cadastrado na frota." << endl;
            return;
        }
    }

    // Se a placa não existe, solicita os dados do veículo ao usuário
    cout << "Insira o codigo do veiculo: ";
    cin >> criado.codigo;
    cin.ignore();
    cout << "Insira a marca do veiculo: ";
    getline(cin, marca);
    strcpy(criado.marca, marca.c_str());
    cout << "Insira o modelo do veiculo: ";
    getline(cin, modelo);
    strcpy(criado.Modelo, modelo.c_str());
    cout << "Insira a placa do veiculo: ";
    getline(cin, placa);
    strcpy(criado.placa, placa.c_str());

    // Loop para solicitar a categoria do veículo até que seja válida
    do
    {
        cout << "Insira a categoria do veiculo (B/I/S): ";
        categoria = toupper(cin.get());
        cin.ignore(); // Limpa o caractere de nova linha
        if (categoria != 'B' and categoria != 'I' and categoria != 'S')
            cout << "Digite uma categoria certa!";
    }
    while (categoria != 'B' and categoria != 'I' and categoria != 'S');

    // Atribui os dados restantes ao veículo
    criado.categoria = categoria;
    criado.disponivel = true;
    criado.quantLocacoes = 0;

    // Escreve os dados do veículo no arquivo
    arquivoFrota.write((const char*)(&criado), sizeof(Veiculo));

    // Fecha os arquivos
    arquivoFrota.close();
    arquivoLeitura.close();
    cout << "==============VEICULO INCLUIDO COM SUCESSO===============";
}

void locarVeiculo()
{
    // Abre um arquivo de leitura para verificar a disponibilidade dos veículos (modo binário)
    ifstream arquivoFrota("FROTA.DAD", ios::in);

    // Abre um arquivo de leitura para verificar se o veículo já foi locado (modo texto)
    ifstream arquivoLocados("LOCADOS.TXT");

    // Abre um arquivo de saída para escrever as locações (modo texto)
    ofstream arquivoSaida("LOCADOS.TXT", ios::app);

    int codigoVeiculoEscolhido;
    char categoriaDesejada;
    bool veiculoEncontrado = false;
    Veiculo criado;

    if(!arquivoFrota or !arquivoLocados or !arquivoSaida){
        cout << "Ocorreu algum erro com algum arquivo!"; exit(0);}
    // Solicita a categoria desejada pelo cliente
    do
    {
        cout << "Insira a categoria do veiculo (B/I/S): ";
        categoriaDesejada = toupper(cin.get());
        if (categoriaDesejada != 'B' and categoriaDesejada != 'I' and categoriaDesejada != 'S')
            cout << "Digite uma categoria certa!";
    }
    while (categoriaDesejada != 'B' and categoriaDesejada != 'I' and categoriaDesejada != 'S');

    // Loop para listar veículos disponíveis na categoria desejada
    while (arquivoFrota.read((char*)(&criado), sizeof(Veiculo)))
    {
        if (criado.categoria == categoriaDesejada and criado.disponivel)
        {
            cout << "Código: " << criado.codigo << ", Marca: " << criado.marca << ", Modelo: " << criado.Modelo << ", Placa: " << criado.placa << endl;
            veiculoEncontrado = true;
        }
    }

    // Fecha o arquivo de frota após a leitura
    arquivoFrota.close();

    // Verifica se há veículos disponíveis na categoria desejada
    if (!veiculoEncontrado)
    {
        cout << "Nenhum veiculo disponivel na categoria selecionada!";
        return;
    }

    // Solicita o código do veículo escolhido
    cout << "Insira o código do veiculo escolhido: " << endl;
    cin >> codigoVeiculoEscolhido;

    // Abre o arquivo de frota em modo leitura e escrita
    fstream arquivoFrotaAtualizado("FROTA.DAD", ios::in | ios::out | ios::binary);

    // Loop para encontrar o veículo escolhido
    while (arquivoFrotaAtualizado.read((char*)(&criado), sizeof(Veiculo)))
    {
        if (criado.codigo == codigoVeiculoEscolhido and criado.categoria == categoriaDesejada and criado.disponivel)
        {
            // Atualiza as informações do veículo na frota
            criado.disponivel = false;
            criado.quantLocacoes++;

            // Posiciona o ponteiro para a posição correta no arquivo de frota
            arquivoFrotaAtualizado.seekp(-static_cast<long>(sizeof(Veiculo)), ios::cur);

            // Escreve as informações atualizadas no arquivo de frota
            arquivoFrotaAtualizado.write((const char*)(&criado), sizeof(Veiculo));
            break;
        }
    }

    // Gera a locação e grava no arquivo LOCADOS.TXT
    Locacao novaLocacao;
    string data, cpf;
    int dia, mes, ano;
    bool validar;
    do
    {
        cout << "Digite a data da locacao: (DD/MM/AAAA)";
        getline(cin, data);
        validar = validarData(data, dia, mes, ano);
    }
    while (!validar);
    strcpy(novaLocacao.dataLocacao, data.c_str());

    do
    {
        cout << "Digite o CPF do locador(sem ponto e sem hifen): (XXXXXXXXXXX)" << endl;
        getline(cin, cpf);
        validar = validarCPF(cpf);
    }
    while (!validar);
    strcpy(novaLocacao.cpfCliente, cpf.c_str());

    novaLocacao.codigoVeiculo = criado.codigo;

    do
    {
        cout << "Insira a quantidade de dias de locacao: " << endl;
        cin >> novaLocacao.quantDias;
    }
    while (novaLocacao.quantDias < 1 or novaLocacao.quantDias > 100);   //defini como 100 o maximo de dias para locação

    novaLocacao.ativa = true;
    arquivoSaida << novaLocacao.dataLocacao << " " << novaLocacao.cpfCliente << " " << novaLocacao.codigoVeiculo << " " << novaLocacao.quantDias << " " << novaLocacao.ativa << endl;

    cout << "Locacao realizada com SUCESSO!!!!";
    arquivoSaida.close();
    arquivoLocados.close();
    arquivoFrotaAtualizado.close();
}
//funcao ja utilizada nos ultimos trabalhos
bool validarData(string data, int& dia, int& mes, int& ano)
{
    if (data.size() != 10 or not isdigit(data[0]) or not isdigit(data[1]) or data[2] != '/' or not isdigit(data[3]) or not isdigit(data[4]) or data[5] != '/' or not isdigit(data[6]) or not isdigit(data[7]) or not isdigit(data[8]) or not isdigit(data[9]))
    {
        return false;
    }
    //retorna por referencia o dia o mes e o ano
    dia = atoi(data.substr(0, 2).c_str());
    mes = atoi(data.substr(3, 2).c_str());
    ano = atoi(data.substr(6, 4).c_str());


    if (dia < 1 or dia > 31 or mes < 1 or mes > 12 or ano < 2023 or ano > 2025)
    {
        return false;
    }

    return true;
}

//funcao para validar o cpf, obtei por deixar em string mesmo, porém validando em numeros
bool validarCPF(string &cpf)
{
    //tem que ter tamanho 11
    if (cpf.size() != 11)
        return false;
    //loop para avaliar se cada char eh um digito
    for(int i = 0; i < cpf.size(); i++)
        if(not isdigit(cpf[i]))
            return false;
    return true;
}

void devolucaoVeiculo()
{
    string buscaCPF;
    bool validar;

    // Variáveis para armazenar os dados da locação
    Locacao locacao;

    // Solicita o CPF a ser encontrado
    do
    {
        cout << "Digite o CPF do cliente(sem pontos e sem hifen): (XXXXXXXXXXX)" << endl;
        getline(cin, buscaCPF);
        validar = validarCPF(buscaCPF);
    } while (!validar);

    ifstream arquivoLocados("LOCADOS.TXT");
    ofstream arquivoTemporario("TEMP.TXT");
    bool locacaoEncontrada = false;

    if(!arquivoLocados or !arquivoTemporario){
        cout << "Ocorreu algum erro ao abrir o arquivo!"; exit(0);}

    // Loop para buscar a locação pelo CPF do cliente
    while (arquivoLocados >> locacao.dataLocacao >> locacao.cpfCliente >> locacao.codigoVeiculo >> locacao.quantDias >> locacao.ativa)
    {
        if (locacao.cpfCliente == buscaCPF && locacao.ativa)
        {
            locacaoEncontrada = true;

            string dataDevolucao;
            int diaDevolucao, mesDevolucao, anoDevolucao;
            bool validarDevolucao;

            do
            {
                cout << "Digite a data da devolucao: (DD/MM/AAAA)";
                getline(cin, dataDevolucao);
                validarDevolucao = validarData(dataDevolucao, diaDevolucao, mesDevolucao, anoDevolucao);
            } while (!validarDevolucao);

            int diaAtual, mesAtual, anoAtual;
            string dataAtual;
            bool validarAtual;

            // Solicita a data atual ao usuário
            do
            {
                cout << "Digite a data de hoje: (DD/MM/AAAA)" << endl;
                getline(cin, dataAtual);
                validarAtual = validarData(dataAtual, diaAtual, mesAtual, anoAtual);
            } while (!validarAtual);

            if (anoAtual > anoDevolucao || (anoAtual == anoDevolucao && (mesAtual > mesDevolucao || (mesAtual == mesDevolucao && diaAtual > diaDevolucao))))
            {
                // Exibe mensagem de atraso e calcula valor total
                int diasAtraso = (anoAtual - anoDevolucao) * 365 + (mesAtual - mesDevolucao) * 30 + (diaAtual - diaDevolucao);
                float multa = 0.20 * diasAtraso * locacao.quantDias + 0.01 * diasAtraso * locacao.quantDias;
                cout << "=============ATRASO EM DEVOLUCAO============" << endl
                     << "Multa de: R$" << multa << endl;
            }

            // Atualiza a situação do veículo na frota
            VeiculoDisponivel(locacao.codigoVeiculo);

            // Atualiza a situação da locação no arquivo LOCADOS.TXT
            atualizaLocacoes(buscaCPF);

            cout << "=============DEVOLUCAO REALIZADA COM SUCESSO===============" << endl;
            break; // Não é necessário continuar procurando
        }
        else
        {
            // Se a locação não for a desejada, escreve no arquivo temporário
            arquivoTemporario << locacao.dataLocacao << " " << locacao.cpfCliente << " " << locacao.codigoVeiculo << " " << locacao.quantDias << " " << locacao.ativa << endl;
        }
    }

    // Se chegou aqui, a locação não foi encontrada
    if (!locacaoEncontrada)
        cout << "Nenhuma locacao encontrada!!!!";

    // Fecha os arquivos
    arquivoLocados.close();
    arquivoTemporario.close();

    // Renomeia o arquivo temporário para LOCADOS.TXT
    remove("LOCADOS.TXT");
    rename("TEMP.TXT", "LOCADOS.TXT");
}

void VeiculoDisponivel(int codigoVeiculo)
{
    Veiculo veiculo;
    ifstream arquivoFrota("FROTA.DAD", ios::binary);
    ofstream arquivoAtualizado("TEMP_FROTA.DAD", ios::binary | ios::app);

    if(!arquivoAtualizado or !arquivoFrota){
        cout << "Ocorreu algum erro ao abrir o arquivo!"; exit(0);}
    while (arquivoFrota.read(reinterpret_cast<char *>(&veiculo), sizeof(Veiculo)))
    {
        if (veiculo.codigo == codigoVeiculo)
        {
            // Atualiza a situação do veículo
            veiculo.disponivel = true;
            // Escreve as informações atualizadas no arquivo de frota
            arquivoAtualizado.write((const char*)(&veiculo), sizeof(Veiculo));
            break;
        }
    }

    arquivoFrota.close();
    arquivoAtualizado.close();

    // Renomeia o arquivo temporário para FROTA.DAD
    remove("FROTA.DAD");
    rename("TEMP_FROTA.DAD", "FROTA.DAD");
}

void atualizaLocacoes(string &cpfCliente)
{
    Locacao locacao;
    ifstream arquivoLeitura("LOCADOS.TXT");
    ofstream arquivoTemporario("TEMP_LOCADOS.TXT");

    if(!arquivoLeitura or !arquivoTemporario){
        cout << "Ocorreu algum erro com algum arquivo!"; exit(0);}
    while (arquivoLeitura >> locacao.dataLocacao >> locacao.cpfCliente >> locacao.codigoVeiculo >> locacao.quantDias >> locacao.ativa)
    {
        if (!(locacao.cpfCliente == cpfCliente) && locacao.ativa)
            arquivoTemporario << locacao.dataLocacao << " " << locacao.cpfCliente << " " << locacao.codigoVeiculo << " " << locacao.quantDias << " " << locacao.ativa << endl;
    }

    arquivoLeitura.close();
    arquivoTemporario.close();

    // Renomeia o arquivo temporário para LOCADOS.TXT
    remove("LOCADOS.TXT");
    rename("TEMP_LOCADOS.TXT", "LOCADOS.TXT");
}

void relatorio()
{
    string dataAtual;
    bool validar;
    int diaAtual, mesAtual, anoAtual;

    // Solicita a data atual ao usuário
    do
    {
        cout << "Digite a data de hoje: (DD/MM/AAAA)" << endl;
        getline(cin, dataAtual);
        validar = validarData(dataAtual, diaAtual, mesAtual, anoAtual);
    }
    while(not validar);

    ifstream arquivoLocados("LOCADOS.TXT");
    Locacao locacao;

    if(!arquivoLocados){
        cout << "Ocorreu algum erro com o arquivo!"; exit(0);}

    cout << "==============RELATORIO DE DEVOLUCOES PARA HOJE===============";

    // Loop para buscar as locações ativas e verificar se a devolução é hoje
    while(arquivoLocados >> locacao.dataLocacao >> locacao.cpfCliente >> locacao.codigoVeiculo >> locacao.quantDias >> locacao.ativa)
        if(dataAtual == locacao.dataLocacao and locacao.ativa)
            cout << "Data da locacao: " << locacao.dataLocacao << "-CPF do cliente: " << locacao.cpfCliente << "-Codigo do veiculo: " << locacao.codigoVeiculo << "-Quantidade de dias: " << locacao.quantDias << endl;

    arquivoLocados.close();

    //exibe mensagem se nao houver devolucoes hoje
    cout << "Nenhuma devolucao pra hoje!";
}
