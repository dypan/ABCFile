//too many parameters, should encapsulate those parameters into class later.
public void SendEmail(string server, List<string> to, string from, string pwd, string title, string content, List<Attachment> attachments)
        {
            System.Net.Mail.SmtpClient client = new SmtpClient(server);
            client.UseDefaultCredentials = false;
            client.Credentials = new System.Net.NetworkCredential(from, pwd);
            client.DeliveryMethod = SmtpDeliveryMethod.Network;

            System.Net.Mail.MailMessage message = new System.Net.Mail.MailMessage();
            message.From = new MailAddress(from);
            foreach (var addr in to)
            {
                message.To.Add(addr);
            }
            message.BodyEncoding = System.Text.Encoding.UTF8;
            message.IsBodyHtml = true;
            client.Send(message);
        }
