﻿using System.IO;
using System.Linq;

namespace Machine
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var сигналы = new Сигналы();
            var регком = new РЕГКОМ();
            var укком = new УККОМ();
            var проводки = new Проводки(сигналы, регком);
            проводки.ЗАПП_Перерезан = true;

            var рон = new РОН(проводки);
            var ир = new ИР(проводки);
            var плюс = new Плюс(ир, регком);
            var память = new Память(плюс, проводки, укком);
            var мвыб = new МВЫБ(память, плюс, проводки);
            var алу = new АЛУ(рон, мвыб, сигналы);
            var мчист = new МЧИСТ(алу, проводки);
            var плюсОдин = new ПлюсОдин(укком);
            var мперех = new МПЕРЕХ(плюсОдин, плюс, сигналы);
            укком.Инициализируйся(мперех, проводки);
            ир.Инициализируйся(мчист);
            рон.Инициализируйся(алу);
            регком.Инициализируйся(память);
            var декком = new ДЕККОМ(сигналы, рон, регком);

            var программа = File.ReadLines("prog.txt").ToArray();
            память.Инициализируйся(алу, программа);

            декком.Старт();
        }
    }
}