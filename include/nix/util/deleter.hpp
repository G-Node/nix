    struct TLinkTypeDT {
        typedef LinkType multi_tag;
    };
    struct TLinkTypeT {
        typedef LinkType tag;
    };
    typedef typename std::conditional<std::is_same<TPARENT, nix::Tag>::value,
                                      TLinkTypeT, TLinkTypeDT>::type
                                      TLinkType;
    
    template<typename T = TLinkType>
    void linkType(typename T::tag link_type);
    
    
    template<typename T = TLinkType>
    void linkType(typename T::multi_tag link_type);
