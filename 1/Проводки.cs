namespace Machine
{
    public class Проводки
    {
        private readonly РЕГКОМ регком;
        private readonly Сигналы сигналы;

        public Проводки(Сигналы сигналы, РЕГКОМ регком)
        {
            this.сигналы = сигналы;
            this.регком = регком;
        }

        public bool ЗАПП_Перерезан { private get; set; }

        public bool ЗАПП
        {
            get { return !ЗАПП_Перерезан && сигналы.П == 0; }
        }

        public bool ЗАМ1_Перерезан { private get; set; }

        public bool ЗАМ1
        {
            get { return !ЗАМ1_Перерезан && сигналы.П == 1; }
        }

        public bool ЗАМ2_Перерезан { private get; set; }

        public bool ЗАМ2
        {
            get { return !ЗАМ2_Перерезан && сигналы.П != 3; }
        }

        public bool ВЗАП1_Перерезан { private get; set; }

        public bool ВЗАП1
        {
            get { return !ВЗАП1_Перерезан && сигналы.П == 3; }
        }

        public bool ВЫБ_Перерезан { private get; set; }

        public bool ВЫБ
        {
            get { return !ВЫБ_Перерезан && сигналы.И; }
        }

        public bool ЧИСТ_Перерезан { private get; set; }

        public bool ЧИСТ
        {
            get { return !ЧИСТ_Перерезан && !(сигналы.П == 2 || сигналы.П == 3); }
        }

        public bool ПУСК_Перерезан { private get; set; }

        public bool ПУСК
        {
            get { return !ПУСК_Перерезан && регком.КОП != "FF"; }
        }
    }
}