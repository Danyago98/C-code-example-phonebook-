#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <list>

std::string NameNormalization(const std::string& string) {
    std::string normal;
    for (size_t i = 0; i < string.size(); ++i) {
        normal += std::tolower(static_cast<unsigned char>(string[i]));
    }
    std::vector<std::string> split;
    auto pos = normal.find(" ");
    std::string new_str = normal;
    while (pos != std::string::npos) {
        split.push_back(new_str.substr(0, pos));
        new_str = new_str.substr(pos + 1);
        pos = new_str.find(" ");
    }
    split.push_back(new_str.substr(0, pos));
    std::sort(split.begin(), split.end());
    std::string ans;
    bool more = (split.size() > 1);
    for (const auto& el : split) {
        ans += el;
        if (more) {
            ans += " ";
        }
    }
    return ans;
}

std::string NumNormalization(const std::string& str) {
    std::string normal;
    if (str[0] == '+') {
        normal = str.substr(1);
    } else {
        normal = str;
    }
    return normal;
}

struct Number {
    std::string number_;
    std::string normalized_number_;
    bool operator==(const Number& rhs) const {
        return normalized_number_ == rhs.normalized_number_;
    }
};

struct Name {
    std::string name_;
    std::string normalized_name_;
    bool operator==(const Name& rhs) const {
        return normalized_name_ == rhs.normalized_name_;
    }
};

struct Adress {
    std::string adress_;
};

class Phonebook;

class Contact {
public:
    void SetName(const std::string& name);

    void SetNumber(const std::string& number);

    void SetAdress(const std::string& adr);

    Contact(const std::string& nme, const std::string& nmb);

    Contact(const std::string& nme, const std::string& nmb, const std::string& addr);

    bool operator==(const Contact& rhs) const;

    Number GetContactNumber() const;

    Name GetContactName() const;

    Adress GetContactAdress() const;

private:
    Number contact_number_;
    Name contact_name_;
    Adress contact_adress_;
    Phonebook* pb_;
};

class Phonebook {
public:
    int AddContact(Contact con);

    Contact SearchByName(const std::string& name);

    Contact SearchByNumber(const std::string& num);

    Contact SearchById(int some_id);

    void DeleteById(int some_id);

    std::vector<Contact> FilterByAdress(const std::string& addr);

    Contact GetById(int some_id) const;

private:
    std::list<Contact> contacts_;
    std::unordered_map<std::string, std::list<Contact>::iterator> numbers_;
    std::unordered_map<std::string, std::list<Contact>::iterator> names_;
    std::unordered_map<int, std::list<Contact>::iterator> ids_;
    static int id;
};

int Phonebook::id = 1;

void Contact::SetName(const std::string& name) {
    if (ph_->names_.find(name) == ph_->names_.end()) {
        ph_->names_.erase(Name.normalized_name_);
        contact_name_.name_ = name;
        contact_name_.normalized_name_ = NameNormalization(name);
        auto it = std::prev(ph_->contacts_.end());
        numbers_[con.GetContactNumber().normalized_number_] = it;
        ph_->names_[name] =
    }
}

void Contact::SetNumber(const std::string& number) {
    contact_number_.number_ = number;
    contact_number_.normalized_number_ = NumNormalization(number);
}

void Contact::SetAdress(const std::string& adr) {
    contact_adress_.adress_ = adr;
}

Contact::Contact(const std::string& nme, const std::string& nmb) {
    contact_number_.number_ = nmb;
    contact_name_.name_ = nme;
    contact_adress_.adress_ = "";
    contact_number_.normalized_number_ = NumNormalization(nmb);
    contact_name_.normalized_name_ = NameNormalization(nme);
    ph_ = nullptr;
}

Contact::Contact(const std::string& nme, const std::string& nmb, const std::string& addr) {
    contact_number_.number_ = nmb;
    contact_name_.name_ = nme;
    contact_adress_.adress_ = addr;
    contact_number_.normalized_number_ = NumNormalization(nmb);
    contact_name_.normalized_name_ = NameNormalization(nme);
    ph_ = nullptr;
}

bool Contact::operator==(const Contact& rhs) const {
    return (contact_name_.normalized_name_ == contact_name_.normalized_name_ &&
            contact_number_.normalized_number_ == contact_number_.normalized_number_ &&
            contact_adress_.adress_ == rhs.contact_adress_.adress_);
}

Number Contact::GetContactNumber() const {
    return contact_number_;
}

Name Contact::GetContactName() const {
    return contact_name_;
}

Adress Contact::GetContactAdress() const {
    return contact_adress_;
}

int Phonebook::AddContact(Contact con) {
    contacts_.push_back(con);
    auto it = std::prev(contacts_.end());
    numbers_[con.GetContactNumber().normalized_number_] = it;
    names_[con.GetContactName().normalized_name_] = it;
    ids_[id] = it;
    id++;
    con.ph_ = this;
    return id;
}

Contact Phonebook::SearchByName(const std::string& name) {
    auto it = names_.find(NameNormalization(name));
    if (it != names_.end()) {
        return *(it->second);
    }
    return Contact("", "");
}

Contact Phonebook::SearchByNumber(const std::string& num) {
    auto it = numbers_.find(NumNormalization(num));
    if (it != numbers_.end()) {
        return *(it->second);
    } else {
        return Contact("", "");
    }
}

Contact Phonebook::SearchById(int some_id) {
    auto it = ids_.find(some_id);
    if (it != ids_.end()) {
        return *(it->second);
    } else {
        return Contact("", "");
    }
}

void Phonebook::DeleteById(int some_id) {
    auto it = ids_.find(some_id);
    auto val = this->GetById(some_id);
    ids_.erase(it);
    auto c_num = val.GetContactNumber().normalized_number_;
    auto c_name = val.GetContactName().normalized_name_;
    auto it_n = names_.find(c_name);
    names_.erase(it_n);
    auto it_num = numbers_.find(c_num);
    contacts_.erase(it_num->second);
    numbers_.erase(it_num);
}

std::vector<Contact> Phonebook::FilterByAdress(const std::string& addr) {
    std::vector<Contact> filter;
    for (auto it = contacts_.begin(); it != contacts_.end(); ++it) {
        std::string cur_addr = it->GetContactAdress().adress_;
        if (cur_addr.find(addr) != std::string::npos) {
            filter.push_back(*it);
        }
    }
    return filter;
}

Contact Phonebook::GetById(int some_id) const {
    auto it = ids_.find(some_id);
    return *(it->second);
}