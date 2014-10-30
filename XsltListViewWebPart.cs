            using (SPSite site = new SPSite(SPContext.Current.Web.Url.ToString()))
            {
                using (SPWeb web = site.OpenWeb())
                {
                    SPList list = web.Lists["cccc"];
                    XsltListViewWebPart XsltListViewWebPart1 = new XsltListViewWebPart();
                    XsltListViewWebPart1.ListUrl = @"/Lists/cccc";
                    XsltListViewWebPart1.ListId = list.ID;
                    XsltListViewWebPart1.Toolbar = "notoolbar";
                    SPView lstview = list.DefaultView;
                    XsltListViewWebPart1.ViewGuid = list.DefaultView.ID.ToString("B").ToString();
                    XsltListViewWebPart1.ShowToolbarWithRibbon = false;
                    XsltListViewWebPart1.ChromeType = System.Web.UI.WebControls.WebParts.PartChromeType.None;
                    XsltListViewWebPart1.XmlDefinition = lstview.GetViewXml();
                    placeHolder.Controls.Add(XsltListViewWebPart1);
                }
            }
