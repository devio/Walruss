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
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Walruss.Updates
{
    /// <summary>  
    ///  This class performs RSA-PSS signatures and signature-verification on files.  
    /// </summary>  
    public class Data
    {
        /// <summary>
        /// The Sign method can be used to generate a detached RSA-PSS signature on an input file.
        /// </summary>
        /// <returns>
        /// The function returns the base64 encoded signature.</returns>
        /// <param name="xml_private_key">XML encoded RSA private key</param>
        /// <seealso cref="RSA.ToXmlString(bool)">
        /// </seealso>
        /// <param name="input_file">Path to data</param>
        /// <seealso cref="String">
        /// </seealso>
        public static string Sign(String xml_private_key, String input_file)
        {
            // Detached signature is located at original-filename.ext.sig
            String signature_file = input_file + ".sig";
            byte[] signature;

            // Use RSACng because we want to use RSA Probabilistic Signature Scheme (PSS)
            using (var rsa = new RSACng())
            {
                rsa.FromXmlString(xml_private_key);

                try
                {
                    // Data file to be signed
                    Stream input_stream = File.OpenRead(input_file);

                    // Sign data with private RSA key, using SHA512 and RSA-PSS
                    signature = rsa.SignData(input_stream, HashAlgorithmName.SHA512, RSASignaturePadding.Pss);

                    // Write signature to detached .sig file.
                    File.WriteAllText(signature_file, Convert.ToBase64String(signature));
                }
                catch (Exception e)
                {
                    Console.WriteLine("[e] Sign error: " + e.Message);
                    return null;
                }
            }
            // Return signature data base64 encoded
            return Convert.ToBase64String(signature);
        }

        /// <summary>
        /// The Verify method can be used to check a detached RSA-PSS signature on an input file.
        /// </summary>
        /// <returns>
        /// The function returns True, if signature is OK, False otherwise.</returns>
        /// <param name="xml_public_key">XML encoded RSA public key</param>
        /// <seealso cref="RSA.ToXmlString(bool)">
        /// </seealso>
        /// <param name="input_file">Path to data</param>
        /// <seealso cref="String">
        /// </seealso>
        public static bool Verify(String xml_public_key, String input_file)
        {
            // Detached signature is located at original-filename.ext.sig
            String signature_file = input_file + ".sig";

            Boolean success = false;

            // Use RSACng because we want to use RSA Probabilistic Signature Scheme (PSS)
            using (var rsa = new RSACng())
            {
                rsa.FromXmlString(xml_public_key);

                try
                {
                    // Convert base64 encoded signature to byte-array
                    byte[] signature = Convert.FromBase64String(File.ReadAllText(signature_file));

                    // Data file that has been signed
                    Stream input_stream = File.OpenRead(input_file);

                    // Verify data with public RSA key, using SHA512 and RSA-PSS
                    success = rsa.VerifyData(input_stream, signature, HashAlgorithmName.SHA512, RSASignaturePadding.Pss);
                }
                catch (Exception e)
                {
                    Console.WriteLine("[e] Verify error: " + e.Message);
                    return false;
                }
            }
            return success;
        }
    }
}
