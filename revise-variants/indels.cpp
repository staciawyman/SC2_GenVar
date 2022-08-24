class indels : public ml_group_list {
public:
    indels();
};


indels::indels()
{
    ml_group_list& l=*this;


    l.push_back(ml_group("BA.1,BA.2","N:E31-"));
    l.push_back(ml_group("BA.1,BA.2","N:R32-"));
    l.push_back(ml_group("BA.1,BA.2","N:S33-"));
    l.push_back(ml_group("alpha,beta,gamma,BA.1,BA.2","ORF1a:S3675-"));
    l.push_back(ml_group("alpha,beta,gamma,BA.1,BA.2","ORF1a:G3676-"));
    l.push_back(ml_group("alpha,beta,gamma,BA.2","ORF1a:F3677-"));
    l.push_back(ml_group("BA.1","ORF1a:S2083-"));
    l.push_back(ml_group("BA.1","ORF1a:L3674-"));
    l.push_back(ml_group("delta","ORF8:D119-"));
    l.push_back(ml_group("delta","ORF8:F120-"));
    l.push_back(ml_group("alpha,BA.1","S:H69-"));
    l.push_back(ml_group("alpha,BA.1","S:V70-"));
    l.push_back(ml_group("alpha,BA.1","S:Y144-"));
    l.push_back(ml_group("beta","S:L241-"));
    l.push_back(ml_group("beta","S:L242-"));
    l.push_back(ml_group("beta","S:A243-"));
    l.push_back(ml_group("delta","S:E156-"));
    l.push_back(ml_group("delta","S:F157-"));
    l.push_back(ml_group("BA.1","S:G142-"));
    l.push_back(ml_group("BA.1","S:V143-"));
    l.push_back(ml_group("BA.1","S:N211-"));
    l.push_back(ml_group("BA.2","S:L24-"));
    l.push_back(ml_group("BA.2","S:P25-"));
    l.push_back(ml_group("BA.2","S:P26-"));
}
