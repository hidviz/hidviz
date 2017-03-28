#ifndef HIDVIZ_USAGE_H
#define HIDVIZ_USAGE_H

namespace libhidx {
namespace hid {

    class Usage {
    public:
        Usage(unsigned usageId);

        void setData(uint32_t data) { m_data = data; }
        const auto &getName() const { return m_name; }
        auto getData() const { return m_data; }
        auto getValue() const { return m_value;}

    private:
        unsigned m_value;
        std::string m_name;
        uint32_t m_data;
    };
}
}


#endif //HIDVIZ_USAGE_H
