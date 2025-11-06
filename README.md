# 🎓 Sistema Acadêmico em C

Um sistema acadêmico simples desenvolvido em **C**, que permite a interação entre **professores e alunos**.  
O programa roda em terminal e possui menus interativos para lançamentos de notas, visualização de médias, quiz de estudos e um pequeno chat de dúvidas rápidas.

---

## 🧩 Funcionalidades

### 👨‍🏫 **Professor**
- Lançar notas de alunos (NP1, NP2 e PIM);
- Cálculo automático da média final `((NP1 + NP2 + PIM) / 3)`;
- Exibir status de aprovação:
  - ✅ **Aprovado** se média ≥ 7;
  - ❌ **Reprovado** caso contrário;
- Cadastrar questões de prova de múltipla escolha;
- Visualizar todas as questões cadastradas.

### 👨‍🎓 **Aluno**
- Login com **nome**, **RA** e **curso**;
- Consultar média (mediante confirmação de nome + RA);
- Fazer um **quiz de estudos** com 5 questões fixas por curso:
  - 📚 ADS — Análise e Desenvolvimento de Sistemas  
  - 💻 Ciência da Computação  
  - 🧠 Engenharia de Software
- Acessar um **chat de dúvidas rápidas** com palavras-c
