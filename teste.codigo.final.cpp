/*
  sistema_academico.c
  - Menu Professor / Aluno
  - Lançamento de média (NP1, NP2, PIM). Média = (NP1+NP2+PIM)/3
  - Se média >= 7 -> aprovado; senão reprovado
  - Cadastro de questões de prova pelo professor (múltipla escolha)
  - Aluno acessa com nome, RA e curso; pode ver média (solicitando nome+RA novamente)
  - 5 questões fixas por curso (sistema) para estudo (alternativas)
  - Chat com palavras-chave "aulas", "professores", "telefone", "secretária"
  - Todas as palavras com acento e cedilha corretamente
  - UTF-8 required
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_ALUNOS 200
#define MAX_QUEST_PROF 200
#define MAX_PERGUNTA 256
#define MAX_RESPOSTA 64

typedef enum { ADS = 1, CC = 2, ESW = 3 } Curso;

typedef struct {
    char nome[100];
    char ra[30];
    Curso curso;
    double np1;
    double np2;
    double pim;
    double media;
    int tem_media; // 0 = não, 1 = sim
} Aluno;

typedef struct {
    char professor_nome[100];
    char materia[100];
    int logado; // 0 = não, 1 = sim
} Professor;

typedef struct {
    char enunciado[MAX_PERGUNTA];
    char altA[128];
    char altB[128];
    char altC[128];
    char altD[128];
    char correta; // 'A','B','C' ou 'D'
    Curso curso;  // curso a que a questão pertence (0 para geral)
} QuestaoSistema;

typedef struct {
    char enunciado[MAX_PERGUNTA];
    char altA[128];
    char altB[128];
    char altC[128];
    char altD[128];
    char correta; // 'A','B','C' ou 'D'
} QuestaoProf;

Aluno alunos[MAX_ALUNOS];
int qtd_alunos = 0;

Professor prof;

QuestaoProf questoes_prof[MAX_QUEST_PROF];
int qtd_quest_profs = 0;

QuestaoSistema questoes_sistema[9*5]; // 3 cursos x 5 questões
int qtd_quest_sistema = 0;

/* utilitários */
void ler_string(char *buf, int tam) {
    if (fgets(buf, tam, stdin)) {
        size_t l = strlen(buf);
        if (l > 0 && buf[l-1] == '\n') buf[l-1] = '\0';
    }
}

Curso escolher_curso_menu() {
    int op;
    do {
        printf("Escolha o curso:\n");
        printf("1 - ADS\n");
        printf("2 - Ciência da computação\n");
        printf("3 - Engenharia de software\n");
        printf("Opção: ");
        if (scanf("%d", &op) != 1) { while(getchar()!='\n'); op = 0; }
        while(getchar()!='\n');
    } while(op < 1 || op > 3);
    return (Curso)op;
}

const char* nome_curso_str(Curso c) {
    if (c == ADS) return "ADS";
    if (c == CC) return "Ciência da computação";
    return "Engenharia de software";
}

int encontrar_aluno_por_nome_ra(const char *nome, const char *ra) {
    for (int i = 0; i < qtd_alunos; ++i) {
        if (strcmp(alunos[i].ra, ra) == 0 && strcmp(alunos[i].nome, nome) == 0) return i;
    }
    return -1;
}

int encontrar_aluno_por_ra(const char *ra) {
    for (int i = 0; i < qtd_alunos; ++i) {
        if (strcmp(alunos[i].ra, ra) == 0) return i;
    }
    return -1;
}

void cadastrar_ou_atualizar_aluno_por_professor() {
    char nome[100], ra[30];
    printf("Digite o nome do aluno: ");
    ler_string(nome, sizeof(nome));
    printf("Digite o RA do aluno: ");
    ler_string(ra, sizeof(ra));

    Curso c = escolher_curso_menu();

    double np1, np2, pim;
    printf("Digite NP1: ");
    if (scanf("%lf", &np1) != 1) { while(getchar()!='\n'); printf("Entrada inválida. Abortando lançamento.\n"); return; }
    printf("Digite NP2: ");
    if (scanf("%lf", &np2) != 1) { while(getchar()!='\n'); printf("Entrada inválida. Abortando lançamento.\n"); return; }
    printf("Digite PIM: ");
    if (scanf("%lf", &pim) != 1) { while(getchar()!='\n'); printf("Entrada inválida. Abortando lançamento.\n"); return; }
    while(getchar()!='\n');

    int idx = encontrar_aluno_por_nome_ra(nome, ra);
    if (idx == -1) {
        // se não existe, cria novo
        if (qtd_alunos >= MAX_ALUNOS) {
            printf("Capacidade máxima de alunos atingida.\n");
            return;
        }
        idx = qtd_alunos++;
        strncpy(alunos[idx].nome, nome, sizeof(alunos[idx].nome)-1);
        strncpy(alunos[idx].ra, ra, sizeof(alunos[idx].ra)-1);
        alunos[idx].curso = c;
    } else {
        // atualiza curso caso diferente
        alunos[idx].curso = c;
    }

    alunos[idx].np1 = np1;
    alunos[idx].np2 = np2;
    alunos[idx].pim = pim;
    alunos[idx].media = (np1 + np2 + pim) / 3.0;
    alunos[idx].tem_media = 1;

    printf("Média lançada para %s (RA: %s) - Curso: %s\n", alunos[idx].nome, alunos[idx].ra, nome_curso_str(alunos[idx].curso));
    printf("NP1: %.2f  NP2: %.2f  PIM: %.2f  -> Média final: %.2f\n", np1, np2, pim, alunos[idx].media);
    if (alunos[idx].media >= 7.0) printf("Status: APROVADO\n");
    else printf("Status: REPROVADO\n");
}

void cadastrar_questao_professor() {
    if (qtd_quest_profs >= MAX_QUEST_PROF) {
        printf("Limite de questões de professor atingido.\n");
        return;
    }
    QuestaoProf *q = &questoes_prof[qtd_quest_profs++];

    printf("Digite o enunciado da questão (múltipla escolha): ");
    ler_string(q->enunciado, sizeof(q->enunciado));
    printf("Alternativa A: "); ler_string(q->altA, sizeof(q->altA));
    printf("Alternativa B: "); ler_string(q->altB, sizeof(q->altB));
    printf("Alternativa C: "); ler_string(q->altC, sizeof(q->altC));
    printf("Alternativa D: "); ler_string(q->altD, sizeof(q->altD));
    char resp;
    do {
        printf("Informe a alternativa correta (A/B/C/D): ");
        if (scanf(" %c", &resp) != 1) { while(getchar()!='\n'); resp = 0; }
        while(getchar()!='\n');
        if (resp >= 'a' && resp <= 'z') resp = resp - 'a' + 'A';
    } while(resp != 'A' && resp != 'B' && resp != 'C' && resp != 'D');
    q->correta = resp;
    printf("Questão cadastrada com sucesso.\n");
}

/* inicializa 5 questões por curso (originais, criadas aqui) */
void inicializar_questoes_sistema() {
    // ADS - 5 questões
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que significa a sigla 'ADS' no contexto de cursos superiores?",
        "A - Análise e Desenvolvimento de Sistemas",
        "B - Administração de Sistemas",
        "C - Aplicações em Desenvolvimento de Software",
        "D - Arquitetura de Sistemas Digitais",
        'A',
        ADS
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual das estruturas é mais indicada para armazenar uma sequência FIFO?",
        "A - Pilha",
        "B - Fila",
        "C - Árvore binária",
        "D - Tabela hash",
        'B',
        ADS
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual linguagem é comumente usada para desenvolvimento backend em microserviços?",
        "A - Java",
        "B - Assembly",
        "C - HTML",
        "D - CSS",
        'A',
        ADS
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que é um 'merge' em controle de versão (git)?",
        "A - Apagar um branch",
        "B - Unir alterações de duas linhas de desenvolvimento",
        "C - Clonar um repositório remoto",
        "D - Reinicializar o histórico",
        'B',
        ADS
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual dos seguintes é um banco de dados relacional?",
        "A - MongoDB",
        "B - Redis",
        "C - MySQL",
        "D - Elasticsearch",
        'C',
        ADS
    };

    // Ciência da computação - 5 questões
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que representa a notação O(n) na análise de algoritmos?",
        "A - Quantidade de memória usada",
        "B - Ordem de crescimento do tempo de execução",
        "C - Número de linhas de código",
        "D - A velocidade do processador",
        'B',
        CC
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual estrutura de dados permite busca em tempo aproximadamente O(1)?",
        "A - Lista ligada",
        "B - Árvore",
        "C - Tabela hash",
        "D - Fila",
        'C',
        CC
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual é a forma correta de declarar um ponteiro para inteiro em C?",
        "A - int p;",
        "B - int *p;",
        "C - pointer int p;",
        "D - int &p;",
        'B',
        CC
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que é recursão?",
        "A - Um loop infinito",
        "B - Uma função que chama a si mesma",
        "C - Criação de ponteiros",
        "D - Alocação dinâmica de memória",
        'B',
        CC
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual dos seguintes é um paradigma de programação?",
        "A - Imperativo",
        "B - Binário",
        "C - Hexadecimal",
        "D - Sintético",
        'A',
        CC
    };

    // Engenharia de software - 5 questões
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que é um requisito funcional?",
        "A - Descreve uma característica do hardware",
        "B - Declara o que o sistema deve fazer",
        "C - É um teste unitário",
        "D - Documento de marketing",
        'B',
        ESW
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual modelo enfatiza iterações curtas e entrega contínua?",
        "A - Cascata",
        "B - Espiral",
        "C - Ágil",
        "D - V",
        'C',
        ESW
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que significa TDD?",
        "A - Test-Driven Development (Desenvolvimento orientado a testes)",
        "B - Time-Driven Deployment",
        "C - Total Data Design",
        "D - Technical Design Document",
        'A',
        ESW
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "Qual pratica ajuda a manter a qualidade do código via integração frequente?",
        "A - Pair programming",
        "B - Continuous Integration (CI)",
        "C - Waterfall reviews",
        "D - Manual deployments",
        'B',
        ESW
    };
    questoes_sistema[qtd_quest_sistema++] = (QuestaoSistema){
        "O que é uma história de usuário?",
        "A - Um teste automatizado",
        "B - Uma descrição curta do que o usuário precisa",
        "C - Um modelo de dados",
        "D - Um tipo de arquitetura",
        'B',
        ESW
    };
}

void menu_professor() {
    while (1) {
        printf("\n--- Menu Professor ---\n");
        printf("Professor: %s | Matéria: %s\n", prof.professor_nome[0] ? prof.professor_nome : "Não logado", prof.materia[0] ? prof.materia : "Não informada");
        printf("1 - Lançar média final de um aluno (NP1, NP2, PIM)\n");
        printf("2 - Cadastrar questão de prova (múltipla escolha)\n");
        printf("3 - Ver questões cadastradas por mim\n");
        printf("4 - Voltar ao menu anterior\n");
        printf("Escolha: ");
        int op;
        if (scanf("%d", &op) != 1) { while(getchar()!='\n'); op = 0; }
        while(getchar()!='\n');
        if (op == 1) {
            cadastrar_ou_atualizar_aluno_por_professor();
        } else if (op == 2) {
            cadastrar_questao_professor();
        } else if (op == 3) {
            if (qtd_quest_profs == 0) printf("Nenhuma questão cadastrada por você.\n");
            else {
                for (int i = 0; i < qtd_quest_profs; ++i) {
                    QuestaoProf *q = &questoes_prof[i];
                    printf("\nQuestão %d: %s\nA) %s\nB) %s\nC) %s\nD) %s\nCorreta: %c\n",
                        i+1, q->enunciado, q->altA, q->altB, q->altC, q->altD, q->correta);
                }
            }
        } else if (op == 4) {
            return;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

void visualizar_media_aluno() {
    char nome[100], ra[30];
    printf("Para visualizar sua média, digite novamente seu nome: ");
    ler_string(nome, sizeof(nome));
    printf("Digite seu RA: ");
    ler_string(ra, sizeof(ra));

    int idx = encontrar_aluno_por_nome_ra(nome, ra);
    if (idx == -1) {
        printf("Aluno não encontrado ou médias não lançadas.\n");
        return;
    }
    if (!alunos[idx].tem_media) {
        printf("A média ainda não foi lançada para este aluno.\n");
        return;
    }
    printf("Aluno: %s  RA: %s  Curso: %s\n", alunos[idx].nome, alunos[idx].ra, nome_curso_str(alunos[idx].curso));
    printf("NP1: %.2f  NP2: %.2f  PIM: %.2f  -> MÉDIA FINAL: %.2f\n", alunos[idx].np1, alunos[idx].np2, alunos[idx].pim, alunos[idx].media);
    if (alunos[idx].media >= 7.0) printf("Status: APROVADO\n");
    else printf("Status: REPROVADO\n");
}

void quiz_sistema_para_curso(Curso c) {
    // Mostrar 5 questões correspondentes ao curso
    int cont = 0;
    printf("\n--- Quiz de Estudos (%s) ---\n", nome_curso_str(c));
    for (int i = 0; i < qtd_quest_sistema && cont < 5; ++i) {
        if (questoes_sistema[i].curso != c) continue;
        QuestaoSistema *q = &questoes_sistema[i];
        printf("\nQ%d: %s\n", cont+1, q->enunciado);
        printf("A) %s\nB) %s\nC) %s\nD) %s\n", q->altA, q->altB, q->altC, q->altD);
        char r;
        do {
            printf("Sua resposta (A/B/C/D, ou 'S' para pular): ");
            if (scanf(" %c", &r) != 1) { while(getchar()!='\n'); r = 0; }
            while(getchar()!='\n');
            if (r >= 'a' && r <= 'z') r = r - 'a' + 'A';
            if (r == 'S') break;
        } while(r != 'A' && r != 'B' && r != 'C' && r != 'D' && r != 'S');

        if (r == 'S') {
            printf("Questão pulada.\n");
        } else {
            if (r == q->correta) printf("Correto\n");
            else printf("Resposta incorreta\n");
        }
        cont++;
    }
    if (cont == 0) printf("Nenhuma questão de sistema disponível para este curso.\n");
}

void chat_aluno_simples(const char *entrada) {
    // Procurar palavras-chave simples
    // converter entrada para minúsculas para facilitar
    char s[256];
    strncpy(s, entrada, sizeof(s)-1);
    s[sizeof(s)-1] = '\0';
    for (int i = 0; s[i]; ++i) {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] - 'A' + 'a';
    }
    if (strstr(s, "telefone") || strstr(s, "secretária") || strstr(s, "secretaria")) {
        printf("Resposta: O telefone fixo da secretária é 11 4356-8895 e o WhatsApp é 11 99987-5644\n");
        return;
    }
    if (strstr(s, "aulas") || strstr(s, "semana") || strstr(s, "quais matérias")) {
        printf("Resposta: Segunda feira a matéria é de Engenharia de Software Ágil.\n");
        printf("Terça feira a matéria é de Algoritmo estrutura dados Python\n");
        printf("Quarta feira a matéria é de Programação estruturada em C\n");
        printf("Quinta feira a matéria é de Análise e projeto de sistemas\n");
        printf("As sextas feiras as matérias são online e você não tem aula.\n");
        return;
    }
    if (strstr(s, "professores") || strstr(s, "quem") || strstr(s, "professor")) {
        printf("Resposta: De segunda feira a professora é a Eliana especialista em Engenharia de Software Ágil, de terça feira o professor é o Ageu especialista em Python, de quarta feira o professor é o Emerson Abraham especialista em C e de quinta feira o professor é o Luiz Gobita especialista em Análise e projeto de sistemas.\n");
        return;
    }
    printf("Desculpe, não entendi. Palavras-chave disponíveis: \"aulas\", \"professores\", \"telefone\", \"secretária\".\n");
}

void menu_aluno() {
    char nome[100], ra[30];
    printf("Login Aluno\n");
    printf("Digite seu nome: ");
    ler_string(nome, sizeof(nome));
    printf("Digite seu RA: ");
    ler_string(ra, sizeof(ra));
    Curso c = escolher_curso_menu();

    // Verificar se já existe aluno; se não, cria registro básico sem média
    int idx = encontrar_aluno_por_nome_ra(nome, ra);
    if (idx == -1) {
        if (qtd_alunos < MAX_ALUNOS) {
            idx = qtd_alunos++;
            strncpy(alunos[idx].nome, nome, sizeof(alunos[idx].nome)-1);
            strncpy(alunos[idx].ra, ra, sizeof(alunos[idx].ra)-1);
            alunos[idx].curso = c;
            alunos[idx].tem_media = 0;
            alunos[idx].np1 = alunos[idx].np2 = alunos[idx].pim = alunos[idx].media = 0.0;
            printf("Registro de aluno criado no sistema (sem média lançada ainda).\n");
        } else {
            printf("Sistema cheio. Não foi possível registrar.\n");
            return;
        }
    } else {
        // atualiza curso se necessário
        alunos[idx].curso = c;
    }

    while (1) {
        printf("\n--- Menu Aluno ---\n");
        printf("1 - Visualizar média lançada pelo professor (exige nome + RA novamente)\n");
        printf("2 - Realizar quiz de estudos (5 questões do sistema)\n");
        printf("3 - Chat de dúvidas rápidas (palavras-chave)\n");
        printf("4 - Voltar ao menu anterior\n");
        printf("Escolha: ");
        int op;
        if (scanf("%d", &op) != 1) { while(getchar()!='\n'); op = 0; }
        while(getchar()!='\n');
        if (op == 1) {
            visualizar_media_aluno();
        } else if (op == 2) {
            quiz_sistema_para_curso(c);
        } else if (op == 3) {
            printf("Digite sua pergunta ou palavra-chave: ");
            char entrada[256];
            ler_string(entrada, sizeof(entrada));
            chat_aluno_simples(entrada);
        } else if (op == 4) {
            return;
        } else {
            printf("Opção inválida.\n");
        }
    }
}

int main() {
    setlocale(LC_ALL, ""); // permitir acentuação conforme terminal local
    inicializar_questoes_sistema();

    memset(&prof, 0, sizeof(prof));

    while (1) {
        printf("\n=====================================\n");
        printf("   SISTEMA ACADÊMICO - PROF/ALUNO\n");
        printf("=====================================\n");
        printf("1 - Sou Professor\n");
        printf("2 - Sou Aluno\n");
        printf("3 - Sair\n");
        printf("Escolha: ");
        int op;
        if (scanf("%d", &op) != 1) { while(getchar()!='\n'); op = 0; }
        while(getchar()!='\n');

        if (op == 1) {
            // login professor simples
            printf("Digite seu nome (Professor): ");
            ler_string(prof.professor_nome, sizeof(prof.professor_nome));
            printf("Digite a matéria que leciona: ");
            ler_string(prof.materia, sizeof(prof.materia));
            prof.logado = 1;
            menu_professor();
        } else if (op == 2) {
            menu_aluno();
        } else if (op == 3) {
            printf("Encerrando sistema. Até logo!\n");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}
