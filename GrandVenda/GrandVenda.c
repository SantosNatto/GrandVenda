#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

#define MAX_CLIENTES 100

void clear() {
    #ifdef __linux__
        system("clear");
    #else
        system("cls");
    #endif
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct Cliente{
    char nome[50];
    char endereco[50];
    char telefone[15];
} cliente;

//----------------------------------
cliente listaCliente[MAX_CLIENTES];
int totalClientes = 0;
int men = 0;
//----------------------------------

void menuPrincipal();
void cadastroCliente();
void clientesCadastrados();
void salvarCsv();
void carregarArquivo(const char *BDados);
//----------------------------------

int main(){
    clear();
    setlocale(LC_ALL, "Portuguese_Brazil"); // Configura a localidade de acordo com o sistema
    const char *BDados = "BDados.csv";
    carregarArquivo(BDados);

    while (men != 3){
        menuPrincipal();

        switch (men){
        case 1:
            cadastroCliente();
            break;
        case 2:
            clientesCadastrados();
            break;
        case 3:
            clear();
            printf("Saindo...");
            salvarCsv();    
            Sleep(2500);
            clear();
            return 0;
            break;
        default:
            printf("Opção invalida. Tente novamente!\n");
            Sleep(2500);
            break;
        }
    }
    
    return 0;
}

void menuPrincipal(){
    clear();
    printf("***Menu***\n\n");
    printf("1. Cadastrar novo cliente\n");
    printf("2. Clientes cadastrados\n");
    printf("3. Sair\n");
    printf("-------------------------\n\n");
    printf("Escolha uma opção: ");
    scanf("%d", &men);
    limparBuffer();

    if (men == 3){
        printf("Saindo....");
    }
    if (men < 1 || men > 3){
        printf("Opção escolhida (%d), invalida!!", men);
        Sleep(2500);
        clear();
        return;
    }
}

void cadastroCliente(){
    if (totalClientes >= MAX_CLIENTES){
        printf("Limites de clientes cadastrados alcançados\n");
        Sleep(2500);
        return;
    }
    clear();

    cliente clientes;
    int campo = 1;
    
    while (campo <= 3){
        switch (campo){
        case 1:
            printf("***Cadastro de clientes***\n");
            printf("Nome: ");
            fgets(clientes.nome, sizeof(clientes.nome), stdin);
            clientes.nome[strcspn(clientes.nome, "\n")] = '\0';
            clear();
            campo++;
            break;
        case 2:
            printf("***Cadastro de clientes***\n\n");
            printf("Nome: %s\n", clientes.nome);
            printf("----------------------------------\n\n");
            printf("Endereço: ");
            fgets(clientes.endereco, sizeof(clientes.endereco), stdin);
            clientes.endereco[strcspn(clientes.endereco, "\n")] = '\0';
            clear();
            campo++;
            break;
        case 3:
            printf("***Cadastro de clientes***\n\n");
            printf("Nome: %s\n", clientes.nome);
            printf("Endereço: %s\n", clientes.endereco);
            printf("----------------------------------\n\n");
            printf("Telefone: ");
            fgets(clientes.telefone, sizeof(clientes.telefone), stdin);
            clientes.telefone[strcspn(clientes.telefone, "\n")] = '\0';
            campo++;
            break;
        default:
            break;
        }
    }

    listaCliente[totalClientes] = clientes;
    totalClientes++;

    clear();
    printf("***Cliente cadastrado com sucesso!!\n\n");
    printf("Nome: %s\n", clientes.nome);
    printf("Endereço: %s\n", clientes.endereco);
    printf("Telefone: %s\n", clientes.telefone);
    printf("-------------------------------------\n");
    printf("Pressione enter para continar!!");
    getchar();
}

void clientesCadastrados(){
    clear();
    if (totalClientes == 0){
    printf("Nenhum cliente cadastrado!\n");
    printf("Pressione enter para continuar!");
    getchar();
    clear();
    return;
    }
    
    printf("***Clientes cadastrados***\n\n");
    for (int i = 0; i < totalClientes; i++){
        printf("Nome: %s\n", listaCliente[i].nome);
        printf("Endereço: %s\n", listaCliente[i].endereco);
        printf("Telefone: %s\n", listaCliente[i].telefone);
        printf("----------------------------------------\n");
    }
    printf("Pressione enter para continuar!\n");
    getchar();

}

void salvarCsv(){
    char caminhoArquivo[260];
    snprintf(caminhoArquivo, sizeof(caminhoArquivo), "BDados.csv");

    FILE *arquivo = fopen(caminhoArquivo, "w");
    if (arquivo == NULL){
        printf("Erro para abrir o arquivo para escrita.\n");
        return;
    }

    fprintf(arquivo, "Nome;Endereco;Telefone\n");

    for (int i = 0; i < totalClientes; i++){
        cliente *clientes = &listaCliente[i];
        fprintf(arquivo, "%s;%s;%s\n",
                clientes->nome,
                clientes->endereco,
                clientes->telefone);
    }

    fclose(arquivo);
    printf("Produtos salvos com sucesso no arquivo %s", caminhoArquivo);    
    
}

void carregarArquivo(const char *BDados){
    FILE *arquivo = fopen(BDados, "r");
    if (arquivo == NULL){
        printf("Arquivo '%s' não encontrado. Criando um novo...\n", BDados);
        arquivo = fopen(BDados, "w");
        if (arquivo == NULL){
            printf("Erro ao tentar criar um novo arquivo\n");
            return;
        }
        fprintf(arquivo, "Nome;Endereco;Telefone\n");
        fclose(arquivo);
        printf("Arquivo criado com sucesso!!");
        return;
    }
    
    char linha[512];
    fgets(linha, sizeof(linha), arquivo);

    totalClientes = 0;

    while (fgets(linha, sizeof(linha), arquivo)){
        if (totalClientes >= MAX_CLIENTES){
            printf("Numero maximo de clientes cadastrados!!\n");
            break;
        }
        
        cliente *clientes = &listaCliente[totalClientes];
        char *token = strtok(linha, ";");
        if (token) strncpy(clientes->nome, token, sizeof(clientes->nome) - 1);
        token = strtok(NULL, ";");
        if (token) strncpy(clientes->endereco, token, sizeof(clientes->endereco) - 1);
        token = strtok(NULL, ";");
        if (token) strncpy(clientes->telefone, token, sizeof(clientes->telefone) - 1);

        totalClientes++;

    }
    
    fclose(arquivo);
    printf("Arquivo carregado com sucesso. Total de clientes: %d\n", totalClientes);
} 