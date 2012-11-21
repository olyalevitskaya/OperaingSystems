using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Errors
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var list = new List<int>();

            for (int i = 0; i < args.Length; i++)
            {
                //Проверяем на существование файла
                if (!File.Exists(args[i]))
                    SafeErrorReport(string.Format("File {0} don't exists", args[i]));
                else
                {
                    //пытаемся открыть его
                    try
                    {
                        using (TextReader reader = File.OpenText(args[i]))
                        {
                            string text = reader.ReadLine();
                            string[] bits = text.Split(new[] {' '}, StringSplitOptions.RemoveEmptyEntries);
                            int result;
                            foreach (string number in bits)
                            {
                                bool x = int.TryParse(number, out result);
                                if (!x)
                                    SafeErrorReport(string.Format("Something is wrong in file {0}", args[i]));
                                else
                                {
                                    try
                                    {
                                        list.Add(result);
                                    }
                                    catch (OutOfMemoryException)
                                    {
                                        SafeErrorReport("Out of memory!");
                                        return;
                                    }
                                }
                            }
                        }
                    }
                    catch (Exception)
                    {
                        SafeErrorReport(string.Format("Can't open file {0}", args[i]));
                    }
                }
            }

            try
            {
                using (var w = new StreamWriter(args[args.Length - 1]))
                    foreach (int j in list.OrderBy(l => l).Distinct())
                    {
                        w.Write(j);
                        w.Write(" ");
                    }
            }
            catch (Exception)
            {
                SafeErrorReport(string.Format("Can't open file {0} for writing", args[args.Length - 1]));
            }
        }

        private static void SafeErrorReport(string message)
        {
            try
            {
                Console.Out.WriteLine(message);
            }
            catch
            {
            }
        }
    }
}