//
//  ISFRange.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFRange_h
#define ISFRange_h

#if defined(CINDER_MSW)
    #ifdef max
        #undef max
    #endif      //    max
    #ifdef min
        #undef min
    #endif      //    min
#endif          //    CINDER_MSW

namespace ISF
{
    /*!
     \ingroup VVGL_MISC
     \brief Describes an integer range using a value (loc) and size (len).
     */
    struct Range    {
        size_t        loc = 0;
        size_t        len = 0;
        
        Range() {}
        Range(const size_t & inLoc, const size_t & inLen) : loc(inLoc), len(inLen) {}
        
        //!    Returns the max value of the range
        inline size_t max() const { return loc+len; }
        //!    Returns the min value of the range
        inline size_t min() const { return loc; }
        //!    Returns a true if the receiver intersects the passed range
        inline bool intersects(const Range & n) const    {
            size_t        tmpLoc = this->min();
            size_t        tmpMin = n.min();
            size_t        tmpMax = n.max();
            if (tmpLoc>=tmpMin && tmpLoc<=tmpMax)
                return true;
            tmpLoc = this->max();
            if (tmpLoc>=tmpMin && tmpLoc<=tmpMax)
                return true;
            return false;
        }
        Range & operator=(const Range & n) { loc=n.loc; len=n.len; return *this; }
        bool operator==(const Range & n) { return (loc==n.loc && len==n.len); }
        friend std::ostream & operator<<(std::ostream & os, const Range & n) { os << "{" << n.loc << ":" << n.len << "}"; return os; }
    };
}
#endif /* ISFRange_h */
