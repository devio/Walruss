//
// Source-Code Spende an den PC-Wahl 10.0 Hersteller
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
//   Update- und Wahlergebnis-Paket Signatur/Verifikation - Beispiel
//   Copyright(c) 2017, Chaos Computer Club
//   Author: Thorsten(THS) Schroeder <ths [at] ccc [dot] de>
//
// Published under GNU LESSER GENERAL PUBLIC LICENSE V3
//   Please find the license text in LICENSE
//

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Walruss
{
    class Program
    {
        // We can hard-code the RSA public key. This is equivalent to using certificate-pinning on 
        // RSA-verifier side.
        private const String VENDOR_RSA_PUBLIC_KEY = "<RSAKeyValue><Modulus>tj2/p4UOtsbxK56qqEpHkgOPa6UhrHHNtaBCn2FvYxJggyjQBRKF6peO6WBHFMOZEg7ML/gZtQR+PPAkx/Vct61HtsybqtuPQelzBN/gfaOgh1x1WaAB8xluc/v7RbgK0kTBGvT0GR3kR732TCDBQF+S8U2uCIZtxtbFcojS9LlaFaJNRHG3ZjifPJhScdzNDyyEdjso3sjhN1gkNhrk9VvmCTZAppD0PvmyL4V8L7XZpnyGyaMpfos73bebssrVOszvHr/8j3tN+WZhmlngx2Fh8JNFleVL7NbWs8ZfM3Lj1Hdd5GvDOKnye7byRnxu6SGme8tanbpi0d6S8heZhw==</Modulus><Exponent>AQAB</Exponent></RSAKeyValue>";

        static void Main(string[] args)
        {
            if (args.Length < 1) {
                usage();
                return;
            }

            // This code must be used/implemented on vendor side when deploying update data packages
            if (args[0].Equals("-sign"))
            {
                String res = null;

                if (args.Length < 3)
                {
                    usage();
                    return;
                }

                // Read XML encoded RSA private key from file (args1) and pass path 
                //    to input data (args2)
                res = Walruss.Updates.Data.Sign(File.ReadAllText(args[1]), args[2]);

                Console.WriteLine("[d] RSA Sign - Result: " + res);
            }
            // This code must be used/implemented in the updater program.
            else if (args[0].Equals("-verify"))
            {
                Boolean res = false;

                if (args.Length < 2)
                {
                    usage();
                    return;
                }

                // <blink>Put this in programmaktualisierung10.exe</blink>
                res = Walruss.Updates.Data.Verify(VENDOR_RSA_PUBLIC_KEY, args[1]);

                Console.WriteLine("[d] RSA Verify - Result: " + res);

                if (res == true)
                {
                    // Signature OK
                    // Proceed ...
                }
                else
                {
                    // Signature not OK
                    // Terminate Program
                }
            }
            else
            {
                usage();
                return;
            }
        }

        static void usage()
        {
            Console.WriteLine("[!] usage: program.exe -sign <private key file> <data file>");
            Console.WriteLine("[!]        program.exe -verify <data file>");
            return;
        }
    }
}
