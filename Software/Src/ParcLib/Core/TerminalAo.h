// Copyright (c) 2021-2023 Stefan Grimm. All rights reserved.
// Licensed under the LGPL. See LICENSE file in the project root for full license information.
//
#pragma once

#include "Ao.h"
#include "StringParc.h"
#include "Shared.h"
#include "tsm.h"

namespace parclib {

enum PsType {
  Wait,
  UsbKeycode,
  UsbKeycodeRepeated,
  UsbKeycodes,
  UsbText,
  BleKeycode,
  BleKeycodeRepeated,
  BleText,
  BleControlkey
};

enum CmdType {
  _LastStep = BleControlkey,
  Pin
};

template<uint8_t CMDTYPE>
struct CmdComparator {

  bool operator()(const char* another) const {
    return equals(another);
  }
  bool equals(const char* another) const {
    return false;
  }

  bool operator()(char** another) const {
    return equals(another);
  }
  bool equals(char** another) const {
    return false;
  }

};

using StatePolicy = tsmlib::State<tsmlib::MemoryAddressComparator, true>;

namespace Trigger {
  struct Timeout {
    void* owner;
  };
}

// https://viewer.diagrams.net/?tags=%7B%7D&highlight=0000ff&edit=_blank&layers=1&nav=1&page-id=P0NerHSrNL67rXnCUsoj&title=SoftwareArchitecture.drawio#R%3Cmxfile%20pages%3D%225%22%3E%3Cdiagram%20name%3D%22Domain%20Model%22%20id%3D%229f46799a-70d6-7492-0946-bef42562c5a5%22%3E7Vtbc9o4FP41zD6V8R14LJC2mU222SWdtE87ii2wtrLlkUUC%2FfV7ZEsYXwIOwaWdcR4S60g%2Bks%2F3nU8XOwN7Fm0%2BcpSEtyzAdGAZwWZgzweWNbZs%2BC0N29zgTdzcsOIkyE1mYViQH1gZDWVdkwCnpYaCMSpIUjb6LI6xL0o2xDl7LjdbMlruNUErXDMsfETr1gcSiFBZTcMoKj5hsgpV12NXVTwi%2F%2FuKs3Ws%2BhtY9jL7yasjpH2p9mmIAva8Z7KvBvaMMybyq2gzw1SGVoctv%2B%2FDC7W7cXMcizY3PDD88CX%2B%2BsH%2FaiVz08M37vXVO%2BXlCdG1isc95hGJEVWDFlsdqPSZRBTFUJouWSwWqsaEMqJkFcO1D0PBHAxPmAsCMX6vKgRLwOqHhAY3aMvWcsCpgADq0jRknPwAt9Bx7hOquVB0sbxSi4W8E8wGWDlOoc2djoJZMd2iTanhDUqFMviMUpSk5HH3GBHiKxJPmRAsUo3qQdYRgyfEmz2TCvpHzCIs%2BBaaqFrPs%2FJbVILYtsqY5z26eYok4R7TtA0phq92rguU4UIB%2FQrQ7RroNbApyYBOBWffd3khY7QklM4YZYDyPGZZI40%2BxUvRgH1EgoBmzhLkk3h1L7kwf2cWlpvsxrldWP5RUZAmzgQSKEdJQkLRI6Z3LCWCMOmf522nCSOxyELlTgfuPLNwMWMxPAQiGXwY0H%2FGkgENwB7Mj%2BNoK3SBqq3AHXeErdOALTyswTEK4M86xfyPFC7m69xXnED6VdGHZxU79Ctov54AefKXsXbqWEsTg3uXNFPJEGiD4wb8yzhPIbYzY%2BhKxK0ZlM2ifDES2E47EmiynJ0Fbo0Ff%2BJtIhnQi%2FpZRN3SywONuNmQ96OmvO8K8lEv6m3z2W2N9gFRbwS3I2zHL4h6wpmPUynn3yG9eXYJQ7XNguW9qp%2BNBU2q3sSCzlR9cnhuj2BrJruNZeHu%2Bi%2BJPhYCwOhZ0R0r9Lx%2BMVbojfQeLT5dz%2FuZ%2FkwzvTmZlGZ6Zzxqt7jrbPtmWv1U33rpbrTG%2BxfZwJlNu3Op8sgXeX%2FwK5%2FxHxni9SV9L%2BhvZMDFd29mfRO%2F2KYCR7csJgLA67X9PNruuOVd3O6s9nLa7vXa3jqzndZ4%2Fyra3rRJl9oekDShSI40zRI9Gw%2BiMMhe3c%2FMgcure303f8fZiqOo1%2FUz6bpb0XVHv5O8mK5b9U1ar%2BvHEuS30XWr%2Fho113W8wf5ayFMZ9b4lyTM97XX93By4uK7rjK7r%2BkLgpNf2c2m7WdZ2z66v2a2fq%2B1N71x7bT%2BcJG%2FR9kZ4O9P2%2BrvUqrZL2ADtBPtkSfzS2YzhsyjKTuT7E%2FjOKNIg%2FY0U6U7667t2swYzjoP38ns1KLEki3UoIi3PUKnE2bS0WuvWAUERi4P7kMQVITcdbfhA6M5VsMJ6NgE0QrZiMaJXhfUYgx6VYk%2FxhoivkgdD1zZV%2BZvsZWi6jirP9YSQFbZ7hTvMCYRXTmC5LYZQK3e6%2BG2%2FrnCVlbSvF%2BeMlK25j1ssyyBAK9yGcjJ0beYg%2FUWI5l9e4pgiQZ7KBGjimvJ7J1OrcLqbyJRXd1KZsPLnVXcVjK05Gk2OOMrjUXMEfMuOIHQzlfoHBuw29vPSuDyneVxF5uUjKPJwF%2Fk3pGb9pemMYyR1%2B2h%2BVpIyQGmIA5Vle7lbSac9no8nXonpQ2NsH2F7VqqlTpGIhldOxIn16kQ8PaGcc%2BdT02L9jWlkjytf5jgnppHrHHH0Qhqdi7l2%2FchgIaW%2BO%2BIqlplDoyT3xhvlvpwCbmdyr9%2BcHaWn3lxegp5WhZ6eeyI9HeeIo67p2XTm4VFJpIA8weVKXj5wJj8%2FyT9HyWuht70G3bO5xGTLPUxkTdp3oNXm5DXLlEbhPp3Mblsujy4otVaZgs7JXB4fcdQ1l%2BtHNzfAQMu4xRFT%2B4MuOWqcxFFjOJk4lfWFPf65NG27wr4kTavsslzvNJqOKiv03evin0XTpk9DapJ7nX%2FSfxGxNYbOyN0jMywlRqPfYKc4aUtj43I0NisT%2Fo6Nr6WxbRxxdD4a48fg39v3X5zPdvh540b%2FhX9%2FNhr%2B52yBhV4jdL0pM1%2B3Gm1W6MMC3b2aXnD96nmVXVH1WL31nD%2FyhhPPKH6csrbqQ703kxKKxT9Z5s2Lf2S1r%2F4H%3C%2Fdiagram%3E%3Cdiagram%20id%3D%22LXat12-hiHekpg5oblxC%22%20name%3D%22Component%20Diagram%22%3E7VrbbqMwEP2aPLbCmEvymEvb1aqVUqXZTZ9WTnCIJYJZcNpkv35NsAFDWprdXMhFqqrMwXjMnJnDGGjA7nz5EKJg9kQd7DV0zVk2YK%2Bh67Zm8P8xsEoAq2UmgBsSJ4FABgzIHyxATaAL4uBIGcgo9RgJVHBCfR9PmIKhMKTv6rAp9VSvAXJxCRhMkFdGfxKHzRK0qdsZ%2Fg0TdyY9A6uVHJkjOVhcSTRDDn3PQfCuoVsNHS5RA3YaMab%2BwW5IKascJgfPl13sxbGXcb156jz3f6LopQfgUxAOhq3h6CZZzP2OZ02jFGKfHdy7nnh%2FQ95CsNZH4eSRjEXg2UqyyTkI4p9z6ixirIM84voc8PCUr7sTBWhCfPdxbfXiJaQjJvzKcMiBNxwywhOkLQ4wGqwv4ksBEIGK58DLXIZ9LSBZWB4wnWMWrrgtpkkrRhSaIfPuPUtbCAU2y6WsrgkQiVJx07m34oqPFXTtll24kd12EFwUu5rKrm6U2QVgA7t2vck1yuROxhfIL9T1an416%2BT4NUv8Dn16gfwaks8z49cq8dulIf6Q2bj9WtM0pT4biOMgo%2FYlZqsHYmYZGsuGyxB22mgBIwH6NCKM0FyKzNjcE1MenXNoGArnJihzLunNU27Wm3K7RPk95r0up%2FXKer6yz4t1UG7DhlG50LHvtONdF7dogDlFHY6IXR3g4e04KJphR5CV460g9XhJ2Iib2q0prFcxMP7di2OtSWMlDZ%2BHa5Q3XvPG%2BiQ9M5XT%2BjgkPNxxlq7B7dIoootwgqt7WIZCF382n6gt7Cib0v9PyhvtFmjy9iISU26b8ncgSyRhiD3EyJu6B96UmdssLVtgnxIe1nR1TbVioFyFdJiEV5y0RTVUuC1WaslvQtfu%2FMp56XQa4X%2BZUpR3yu%2FRRaHcvh9GFOyLUgVrf6oAbWiqqgBadZGF4p7XLt4i96QLqQ585HfXuiDpKAT0fGRiwy7wIDKh71YmjOPIhHH85gECq6l2tbURCV0t1qZst%2FctEs0Kv9fmoUoVys8OTnFHUXNR2GPvADSgioJcVA1UoVWozgNtKQyzwu9VFapUofx46RRVwTQVVVg%2FujqELJg1kAVDvgKXsgDrIgtGQRZs%2BT50z7JgmRV%2Br7KQycLYHf360aLt8e%2FFfe95FXRfv1viw4G8KgzIfMFzh3782PkMXyOZha3pCb5G2kgvKNF7iqJva3sX%2Fc%2BK46iiD8zCcyS7LqJvalBZGdQPI%2FqGpt4GTWhcvOivx1Z81SZXkX1OCO%2F%2BAg%3D%3D%3C%2Fdiagram%3E%3Cdiagram%20id%3D%22cTtKM-cefaT5YGLItZzo%22%20name%3D%22ParcApp%22%3E7V1rc6O2Gv41nrM9M8lwx3yMk2zTadKmzW67e75ksJFtWmy5mFzcX38kkDBIwhDH2IJoZi9GvCAhPXrvkgbm5eL1x9hfze9gAKKBoQWvA%2FNqYBiuZqF%2FccEmK3A8OyuYxWGQFenbgofwX0AKNVL6FAZgXSJMIIyScFUunMDlEkySUpkfx%2FClTDaFUbnWlT8DXMHDxI%2F40j%2FDIJlnpUPD3ZbfgHA2pzXrjpfdWfiUmHzJeu4H8KVQZF4PDGdgmK%2F%2BwBwNcFn5j3kZQ5jUklHixesliHDf0379ZfZL%2BPpN%2F%2F74x9T5G%2F72x5%2FXf23OssZ8PvBb816KwTI5eu1mVvuzHz2RUUNvj1A7Rivc78mGDKbzzxPuz9HCj2fhcmBeoLva6jWtapQOGS4%2FS%2BAqu2cV7iXgNTnzo3BGnpugDwVx%2BXYAJjD2kxASmqdlAOIoXIJt1ejXjPyfNnBMCy7hcp34y2SdPToNI5DTxOxTqAvHgrIVWzbnntyrQyIwTbKbQ3yT%2FZb7cPl4kT6DXqm5fLMxwSgncMQElzmBLSa4ygksMcF1TqCJCe60nEKvoNBzCqOimVsKvaKaSyMn8SooTL43ROPXxmih8rQJ1q7%2B%2FroeP36%2Fvt1%2BqqC%2FMM2vv28HXjfEn2KUPsJ4BnESIv56kc2lq3S2jcjMusoaP4KIahql3BLNhAiVTOEyIdIBVZRdf%2FYXYYTlyg2IngF%2BK8Z8sogwEaZBj17CCMZpzeZ0CpzJBJWvkxj%2BDQp3Atcba1re2lo2Rtgd%2FhTwWpATzdjalrn9COACJPEGXZPXWEMiLTZlMfiylT06LZsX5I5hkkKfyLtZ%2Fuo3MVxES3juYVm03QMW%2FfMSvix%2FBpsJTDWSi7Th8RNSOjrCqNGM%2Fd0PwtftlHWK31mc2egzL9FnXqXKHGGqr1dWDfkXf1wgH5k15Nf5%2BJAHtJoHHlb%2BBOQP%2FCf9K3piFIFGH4rohB9qXdaQlz%2FUGNWQsx9qDGseKH8oeqDYIMVe92avjsawV1fAX4cC%2Fqp7cvPXIcdfLwJ%2FGj%2BFyePd5T2SDtqQg8r6JVxEPmJ9ZPjJHb2AF8JCzZEYVpN5GAW3%2FgY%2B4W5C%2Buvkb3o1msM4%2FBe91qdwQbdjijHDKVE84CdRMcZIDNaI5p5iRGeK7vzXEuGtv05IwQRGkb9ah%2BP8MzK2OIJJAheEiAPt1KgArTN2bEcG0Jo6A1pbAFpHAFpHbszSzyopBQxGU9FMR4ca4ELmQ8aR4XQsbhdhEETpGxGHDZezLxjHV2f6tuQ2ffDK3Jb8TnoUF8Uw8RM%2FQxiGU%2BSPQXQP12GmUlzFGe1oBcPUlrsa2KOBfZWWxElq5MV%2BmAIJIOS%2BAIze94NveHjwEbDhx5tgbSg31HQB1PBrY%2BAHP97%2F9OunH4g%2BiMZm%2BJhwOMTK486B2hOPGR8tQ8%2FioWeVhfYcoRgsBXAsww7pU8aldm5jACINwh7p2%2BtaTJ4ceabVDHkUobJCz%2BCgh5S8lR%2FcvCiJLJTI8quRVGukSG0oj4dyI9WwOKR%2BXQMOpGAZXGDHOrqCq5QLoZKi7RD46zkIyIAX7AU0WvHmGxn19OI7vjg3bHp99Vq8e7UhV03Hm8w0hOoZ2EVI%2FMUgKLn%2B3wEL0cDHIPKT8LkcRhCN%2FFvq3dZ%2Bjzn9FpF5tVRsM6rfGj7FE0AeegPaaqo1jd3VZkOxb7UE6nlHn3qC6PwE2U9hXcKUqJO66hsno1JJqzpI5BLFr12FSxxFpd6eXCPNHD5UUX2G6O2NtVQqOatkbd%2B11BbR2BM1lXJuHo5Iq1sjcb4bjmMIIwVHCeBIoddxOOoOB8cHOE1eUM8%2BgDj0ebAp26mb3kzDaohYya0nnXfB76ccKm9mPvmV7ljRQV6lsEaj8WmCBwZdTeZ%2B%2FN%2B99MWP7tVsEYF90RerPOopBKOlAqHMIOyJlmjwmZ86hy%2FeX1nySZZcl6lWR6mD0F%2FAZfBljjNuSgqfbtGCz2n6RPaqYAao1okGfw5ncOlH19vSOsCOiWY3Aq9h8o2%2BFf3O3KQ2udo6SfEF9ZG27lwl%2FZr6EXfREbdGrROWSNjDOWHJDKEtpex2cApvLNVpSSMsNnukLXesVlNvv%2FyxBu9Cy0JrF1CZh1IEymhkLA8PNFW%2FJZc77oFsvQ8QCMgnqTLmKjpI5DjAr42gH7zd3U8g9XaUdV5fbhFp3TPaZr%2Fd%2F2lcja7DZHR2OR4vR7ejZ7L8SpAF9bRUQJMCaH0xzHgH1WkMM60Vw2xrjH0v2mI1htm5a1ll48xzjBrrLL26B3GIRhfrpm2YbEZTk83Q%2BmyzMbqqxa6oaslmM6qWJPTTZjN5v2G%2BUmGEitJft9ePD%2Fc%2FKRuuJyE%2Bs2mIT6eBcUnFmilaaa5ifPugz2wBfr0yC01RsiF%2B7RggFrFfGthHj6i0CLruWYjiHqrMSlSRZdkh2BPb0eRTv1RkuTMgHDYUvrKDkPfw34TBKOKXwyijpBtrthjzXmiTeAKgSm%2BTiHLBVCxq96w%2BjtEhgpPcRgedFJzsXYNlQPYm%2BUT%2BVwtR5EKgyAIRIVByyWu1bYEoCB7VAukiBNu3QBQIj2qBdBGEKreVC6Geaeea7g3KCa5OXtBeDLV%2BSwEifHsZGs2X9dNGuF65xrZCo8xmmDaNK%2FUzNGrxPof7GM5if6GcDlK4EAzHLuHRFWz8ku8N0yUfgqV8CI3hZR3VhyCEk9w%2BBLo7Iac9B0%2FZzrs0dol5QNOt1j5usmGLeBN4DIR4k1xTtquWIvqrFdaBE7D6RAQp%2Fk0NtmKRAuHJQCjwGXQShEaVzyDyNyq5WgqoCTwDnYQa7xm4CdWaN2msBK1stNo2h7pOJj%2FaIse8MhJ2T1GV3VjRQVUudrXoTRqk9SSl0anSzNSqN1mQ1r3MReHyyuotcVJLc%2F2pbHkqq%2FN0iOtJmqLNrx2nYOPAVYgVjiOI1XEcISwE%2BYqhw%2FfG%2BzLsiYN9Rjd2sqHoq439UYu1tdjfkaN9zIJEhz1Gpq3Na5yaevsV7bMbLZFGkLrFhgs1hwv8uDBdmwbfm%2FJdwlXImbqkzkFu2bZgNL8Z6hIwX4rPwghGcDbDJ73Jw3r3W6pe5ryyMl6Kj8MzXu18qGneSbgvs3bWpjHn1nMtvHK9JqP5vI%2F70nrRNPA3hbcSpXKv3qv8Fp3tQ%2BK62qfJzSsh1wethF21cJRKTN18xwij57JBfh8sKUDgdLoGfZD3Dm8dyiLvqSBT8n73CPIrsuWT94fYM%2FTI8p7CrFbe6y3Ke7ec2pUfTnlk6c%2BcLObQBTktS3%2FLrKn3MNK%2Fz%2Bydj45Jw94txd6bjKAoJJUdehOEzwNy4A3l%2BPlR3IV7nZcDOL9%2B62YjbjczP7vvlHuUDZvKCK8tGXEaZxxduXJkZ5xl19TbL2ecw6feS8O9HcW9m4wgHwmRTznfc99ITTeLPBlxaV2v09GPwZTpIr6TOuocwzFLrIqeQHDk7SOZtUoOu41jW1ETq6beQ2vuOY%2Bwhb3eP8Ve3jiNo%2BI0TUbQ5T1vD5t1AhZ3cBkmMFaZs5JkzjpM5NmmIZiOHxfhHmrb0A%2BQOeu2AK9eZc66VfuCqsxZaZDWvcxZYT6jUYE0lTgrC9C6lzgr7iHeeS6h8V7yqDZNYsydpycJrVHofTS3qWWUNUmXXrdsjdvDmno7lEVjM%2FmYtmHt3eTGlRhD9x39slfuiWwOB5ePQ8nicKB8Wjkcdo8gH014AHHoR4%2FcOH5IR0MPjjSxqJu166t6Xd69yYFUHWnSjDsed3fh7rkuqHLEG5Rg67rAe2o2sygHaif%2FNjHXPSeGGHTVqzLViSaSQ7An7o1hlSNNnWjSARD2ZKnwkA9CfQHxIlwiFKgAqCQBUMbKcDWjod4nOfbU1kGN0TVUWwfVgEmUp60CoFIhrXu2gzAAKsrbUAFQmYDWFwuht9nL%2BrmmOYPyipJjnNhQGxodyrDLgGdoTknjO9G6Q5tZ7uEeKXvZZRXedrKXjxIvddilOm2s1WcH6iiVmNrRg7J823qXWD7kIy%2ByxHmHKrG8yQjSCS%2F57mvlxKVO5C1RG69WOFNO1YZwRtJpWBbOJAOmOUgPI5yZxflDmibfsnBmBRpXb0tLi4xypzNnTfWG%2F3vybghDB17x%2F90jyMd1fgabMfTjQGX69CXTJ1%2BU1HEfvFflzJqGIAoyZ1ayWQEOuCrmWDf9T%2Bepl9yt5YnWKu0T91HJZS2CjR5d1O2wkFd1bjhq5BwGnzBv%2B0GxOanYXE%2B8954oIsntHPW0Hsu%2FbVTJV9AJT0FTNz7Vf9rwFNgO5aG5p8B9ozlzGE%2BBweiu7IHJbXkK2M1PDntQM19v%2F5wB8m5ARbmbcgbsHkE%2BhIt7Slbevu8pHLKyd8tpi72fZgErsxHskDq6W%2Bbk7DInj6m2Q%2FFYNrSs7x%2FEbNxdpveOXunF6lVP3qimp6KaTUZQpw7PwhCu0%2BWrUgmzrQDb11bBu926bo08kyoVia5lamcfRSl2QHfYEy889%2Fw4J6C4Xm3NHZJ%2BDpsoRLjcQcWfxdp9x6jEJo5IlY10WK4vbzg6l0hKcteMIR%2BQ%2FrreaYPCVeplRiUkVqwb2Gvpr%2BcgEMjwXPi67xW%2BdZvKn0rE2u1JWMO1yjsV71gkcWShy1ibLmW5Rxa5LWUYHUXgDitM55OKqY0QP10UWs5f%2F%2Fsy%2Bjb%2F6SX8DhZfkXJm%2FPvXmcaxOxphWQ2KHM%2F55wni0ixDJQsCaqvXtKo0RJiWn6UWBr5nFe7huOAZEWjp%2FhRZkk7pdgAmMPazADGmeVoGIE4D23nV22BP1sAxLci2Ir55yZ5cJ%2FHTJMmJYvYx1JNjQdmKLZtzT%2B7VI5kAxzeH%2BCb7MWgOadMYgDuwgPGGLolKocd%2BACZ98ePlLQh%2BZRZP7SKdTkW0oi9u4%2Ft2jkOhBYx8FVuujFJUCPZOU4M5SwsrSGF8%2FdlfhBGexTcgegb4rWWJzAWlgVORYOV6Y%2B3tgrUFzYrdI88SJVhRmmIU2GUyQU%2BnfQmZEa8%2Ft6J7lQ722UPxorrbaXWvXez8cLrX6TUr1zHPNatsJNvmucfk0rSlX1lln47Z4eVVrH5FY90fw9cunC9SW3tFPnMCg0%2B08%2B1xmM7W4NM9pzz1z3RpTD6Xdd3RDJa2WRJjalqOd250183qMsfv2NrRuVJl2%2FRSy7pjA6a0McSaeCUlbUXsr%2BZ3MMAOyev%2FAw%3D%3D%3C%2Fdiagram%3E%3Cdiagram%20id%3D%22p68beTlAC04PYR8i4gAY%22%20name%3D%22Queues%22%3E7Zxhb5s4GMc%2FDdLuxSYwIUlflqRbT9eeesukba9OLjjgzeCccZpkn%2F5ssAOEpKFV1hBAqqr44cEE%2B%2Bc%2F%2F9iAYU%2Bi9ScGF%2BE99RExgOmvDXtqADAyB%2BK%2FDGyywPDKyQIBw34WsvLADP9CKmiq6BL7KCklckoJx4ty0KNxjDxeikHG6KqcNqekfNQFDFAlMPMgqUa%2FYp%2BHWXTsmHn8FuEg1Ee2TLUlgjpZBZIQ%2BnRVCNk3BhgawF5Dw3YNGSv%2F2RNGKT%2BappOj9QQR2fa6XbPjfnx9BdtzZyjmJ6rz44P%2F7z%2B3TuIOZj8SMnOTKPHeW1eqifhGtzvyRTeoImU8pAGNIbnJo663ZE9I1mqJAqPL2E9L4qhuvsMdpQuV8gNxvlGAwSWnIhTyiKitaI35t8Ln77KqD44qTdeq5rSw0YWYs823PFEWvxe35bulJb1ftVlVSyd0yTz0XEOpEQBZgPhzeeMsUbZi4Qj1ei3vu0%2BIRkh8c1FmiECOn8qjAqrBFWzzXgSHyFV8nBYn4DQGJ%2FPDyCkQZf0WnhB5TIXlCTGOhXS9CWIyq7uIqW%2F3BMlSfd%2B%2F0GYB%2FWtaQS9Z4YjAWDI2pzHXFEoUIMFBLD57opcQEwHdg9dqA5e4uV6IiX8HN3QpOyTh0PupS25IGf4lqoWaPLGZccWlOOFixkzuqZhhKBE5DxoQayd0D9elxDuYcBXwKCFwkeDH7WlEghUcu5RzGh2TOHmGaH0aYFQ1A3WB3ZSLq%2FxybQ1VLCxcqsfDRhMGKoRVyCI4pSrhjP7cuhPZIXNMyIQSKpCaxjRN0qgRNOd7QIuw76cymCygh%2BPgS6pz7608cpfuOLXzyGfVlnYqmBxymCEh%2B5%2FAR0QeaII5prJ%2BluW6C4pjnja44xrONI0wPqGxOAmIU1aQQG2FJG57KHp2MJ4QLcUSGNZkqdEo2XtQkrUSCv13fxj2tdxIRRW7gInm5FvAdoB6OWOZmJVxGlRxkiEq9p2T9KIWCjJRvAexMkquaJGJvG4KqMBElK283HzO7EE9zkCzNWtwADS2jHvOmsCZZurCOXMqnN3i3nqdwXo5oJXea9h7r7pa5ZyerVZ5r1HvvRrOWUu817j3Xs3mrCXeSy%2FMFECbbRKOonsaY05Zb8POb8OAWZO1hvswqzrN2huxY%2BOyd2IHGmjfjGpvxRpFWku8mHVoxrU3Y00hrSVuDNgVhs631A2KS93b1e0XLXVbxlsudSu1Ob7WrWrs6Fp3dVr%2FC2IRFpD0Zv%2Fsy91g3BKvX53U773%2BsRHZe%2F0DDbRvBr%2F3%2Bo0irS1e%2F9AMf%2B%2F1m0JaW7y%2B2SCv%2F5q7WotGv%2BD7T3%2BX9Limrx912tZXV4xm0kzLRmc0YDASn%2F5bIrGx4vFDuJAfRedwDMln5HEYBylvqxBzNBOiI7evGFyUAcrUK3PaauCZ%2B%2FXuLS311kMrwdhnqfXoKwmG2egeBoMGCcbrJgfA%2BWYHto8bHX3YossqAqpLNDMkNeThz787ox%2B7P8n3LL9dXaB8DBskH696imZ0vsdo9OzFcfUYdFo%2BqotymXBAwhD05en4aI5jAVFXxMSyx0fVxNLPn16QnNjmhcvJy36%2BnFZNRnXVBHRaTapzMV8ZjQOjW3bE1orxnILo2CUpSHVFoBkKUvfnzDkVpO6ciL53oaMKUp0UedhOhkxCIQuoMzKyOyti69sxLtyIgAt928Qp5aD2nQ%2BdniK1rcskpRE31wzrItayd5WkuUfe3KPqK7wyyb75Hw%3D%3D%3C%2Fdiagram%3E%3Cdiagram%20id%3D%22P0NerHSrNL67rXnCUsoj%22%20name%3D%22TerminalAo%20state%20machine%22%3E5Vpbc6M2FP41nj4lgxBg5zFxLtuZ3c5O3dkmfVOMDMpiRIRI7Pz6CiMBMgIT28Tp9iETdJAEnPN95yaP4HS5umMoCb9RH0cj2%2FJXI3g9sm0AHFv8yyXrQjJ2YSEIGPHlpEowI29YCi0pzYiPU20ipzTiJNGFcxrHeM41GWKMvurTFjTSn5qgADcEszmKmtK%2Fic%2FDQjpxrUr%2BBZMgVE8GlryzRGqyFKQh8ulrTQRvRnDKKOXF1XI1xVGuPKWXYt1ty93yxRiOeZ8F06u%2Fnl7AZfbNmq04DJ6v51P%2FTO7ygqJMfvDvvvj04oX5Wmkh5MtIXIERvEIRCWJxPRePxUwIXjDjRCjsUt7gNBFSRrPYx75a9JjSKOP4ks2liTfSamSJoY%2FScLMiH7yGhONZgub57VeBLSFrfrF6ffEKeFUTSQ3cYbrEnK3FFHnXk8aQaLyQw9fKtOWUsGZWW9kbSTgF5c6VxsWFVLrZANi%2B8dDDPbxJf%2FzzJQOzmXPnnNnjhrKxLxAoh5TxkAY0RtFNJa0pN1dVNecrzXW%2F0e0T5nwttYsyToWoZkShQLa%2Bz9efu2r4ILfbDK5X2mitRivCa8vE6EHtKK6rRflArWm1WkozNpffnD1x9nbx9Dy5fxbU%2F8P5ESzImXQdHLEA8w4Qy3m53joxwHCEOHnRmW2y6GbpJWNoXZuQUBLztLbz91xQQQu4Ew1bCly3bfMtq2u%2BuCjeoMJW%2BSkHwG3SIPzIvfptNBZ%2F7rURil%2FRo%2FDpGnx2%2BoAl8f0CqTglb%2Bhxs18OBqlEsbl7lT%2FQBI9OnjSoXrp%2B%2BRTNu5pcgHVu2batqV65wfcBpGHRMdB39Vx9B7pYpALI2y7jGGa1jKb7MC%2Bi%2BZDKpbR4kV4eoYvpdY9g1Afwju0S%2Bjr5rteuke5PjHwSB98ZDRhaTkXa9OvGXKj7OcdpBl3gGIKuM1jM9U7KFqCzxd5FFxV0VZitkeywoGvEKuwZdMGnYhjcwbAZx0n6C1NsK691rZNzzP1MHBv35Zie2FoDccz5T3LMaeUYif8%2F1Dp9%2BAIXp6BWL7Qb39f77Gjveut6odTQehSRJM21uyBRNKURFei%2BTjmjP7Ec9dJgfzCCiQ5GaOhfmNoXg3n5ZmYrCrukiLltBeXQMWCPHsWglUxnhbKTFfDYrDDXrmUrTCFru3gtvkiuMrU1PqDKhZ8HT0dIDQ7DxdG9ZQsuvP1wsYfNjT1A1fbRHAyOhNc9ZbuqdxeqT2cLdrr8M%2Bvcc%2BBYM8Kh%2FarhqQqMdktEonjaoHD0GvowEjs9SVxhwfK88ac3fjN3%2BonXCfJHeecFblJ3EicZbyCBhXT5mKXHSZYajSdon7uNfMl1m%2FmS67Zr87B86aJJi20dmOqYujNLk%2BKcc0FWOSOOkla6pWakrgAwVDmGzBIMlVnCZkdnc1ABb80HFbF%2FmZ%2F2ilFMY6yrTPce%2B8TztvOyss1wSH7Y9%2ByrE1E7XUgd8gZDKtmB6cJZCRx1qrV9crozjywxqW8EtouYQjWDJR7NymZBWMprLmweIrbTgxkyhQYc2nm55cHAZPyBHsyolmYKfrJjpJLI747pW2TZr4mnfo1SY2g7kI6el293Apzxnjwr%2B1RtGw3MM3eoiHgQ8%2ByxHg%2FtD4yHRjUZ2lEdddD7o%2BF7EuPDuiV9yLeTVOp0oVcY7aytynxa%2BViDgx0uXgJP%2F9EIuJjsGS636vQydOyg8XHqAjGsfshWTK9%2BDghv%2FgU%3D%3C%2Fdiagram%3E%3C%2Fmxfile%3E

template<class PROGSTEPFACTORY, class TSERIAL, class TLOGGERFAC, class THIDBLEFAC, class THIDUSBFAC, class TPROGRAM, class TSYSTEMHWFAC, class KNOWNKEYCODES, uint8_t BUFLEN>
class TerminalAo : public Ao<TerminalAo<PROGSTEPFACTORY, TSERIAL, TLOGGERFAC, THIDBLEFAC, THIDUSBFAC, TPROGRAM, TSYSTEMHWFAC, KNOWNKEYCODES, BUFLEN>> {
  public:
    TerminalAo(TSERIAL& serialInput, Messages& messages, TPROGRAM* programs)
      : Ao_t(messages), _serial(serialInput), _programs(programs) {
      _statemachine.begin();
    }

    void load() {
      if (_timer.increment()) {
        _pinDefinedMsg = Ao_t::_messages.fromKeypadToTerminalQueue.pop();
      }
    }

    void run() {
      if (_timer.current())
      {
        _statemachine.dispatch(Trigger::Timeout{ this });

        if (_pinDefinedMsg != 0) {
          _serial.println(F("PIN was not accepted. A PIN is already active."));
        }

        _pinDefinedMsg = 0;
      }
    }

  private:
    bool stateIdleDo() {
      char ch = 0;
      while (_serial.available()) {
        ch = _serial.read();
        if (ch == '{') {
          // send blocked to KeypadAo
          _serial.print(F("> {"));

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);
          ch = 0;
          return true;
        }
        else if (ch == '?') {
          _serial.println(F("Programmed:"));
          for (int m = 0; m < 4; m++) {
            for (int n = 1; n < 6; n++) {
              KeypadRegData input(m, n);
              _serial.print(input.mode); _serial.print(F(" ")); _serial.print(char(input.button - 1 + 'A')); _serial.print(F(" : "));
              _programs[input.programIndex()].hasSteps() ? _serial.println(F("Yes")) : _serial.println(F("No"));
            }
          }
          auto sysHw = TSYSTEMHWFAC::create();
          _serial.print(F("Remaining bytes: ")); _serial.println(sysHw->freeMemory());
        }
        else if (isprint(ch)) {
          _serial.print(ch);
        }
      }
      if (ch != 0 /*&& ch != 0b1101 && ch != 0b1010*/) {
        // YAT sends 0b1101 and 0b1010, PuTTY sends 0b1101 and powershell sends 0b1010
        // Debug: Serial.println("Enter pressed");
        // Debug: Serial.println(); //eol of repeated command
        _serial.println(F("Start defining your dull programs."));
      }
    }

    KeypadRegData stateReadingProgramCodeEntry() {
      auto log = TLOGGERFAC::create();

      while (_serial.available()) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == ':') {
          parclib::trimFront(_buf);
          parclib::trimBack(_buf, BUFLEN);
          parclib::squeeze(_buf);
          // Debug: _serial.println(_buf);
          uint8_t numSubStr;
          char* subStrs[2] = { 0 };
          parclib::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          if (numSubStr == 2) {
            if (CmdComparator<CmdType::Pin>()(subStrs)) {
              _keyPadInput.isPin = 1;
            }
            else {
              // Debug: _serial.println(subStrs[0][0]); // Does not work when backspace is used in putty
              // Debug: _serial.println(subStrs[1][0]); // Does not work when backspace is used in putty
              // Debug: _serial.println(subStrs[0][strlen(subStrs[0])-1]);
              // Debug: _serial.println(subStrs[1][strlen(subStrs[1])-1]);

              _keyPadInput.isPin = 0;
              _keyPadInput.mode = subStrs[0][0] - '0';
              _keyPadInput.button = subStrs[1][0] - 'A' + 1;

              // Debug: _serial.println(_keyPadInput.mode);
              // Debug: _serial.println(_keyPadInput.button);
            }
          }

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);

          if (_keyPadInput.isPin) {
            log->println(F("got pin"));
            return _keyPadInput;
          }
          else {
            auto idx = _keyPadInput.programIndex();
            // Debug: _log.print(_keyPadInput.mode); _log.print(F(" ")); _log.print(_keyPadInput.button); _log.print(F(" > ")); _log.println(idx);
            if (0 <= idx && idx < NumberOfPrograms) {
              TPROGRAM& prog = _programs[idx];
              if (prog.hasSteps()) {
                log->println(F("got program index. Current program is being disposed."));
              }
              prog.dispose();
              return _keyPadInput;
            }
            else {
              _serial.println(F(" This ain't dull, bye."));
              return KeypadRegData(0);
            }
          }
        }
        else if (_itBuf < BUFLEN) {
          // omit preceding spaces to save the buffer for the data.
          if (_itBuf > 0 || ch != ' ') {
            _buf[_itBuf] = ch;
            _itBuf++;
          }
        }
      }
    }

    bool stateReadingPinEntry() {
      while (_serial.available()) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == '}') {
          _serial.println(F(", thank you."));
          return true;
          // send unblocked to KeypadAo
        }
        else if (ch == ';') {
          // create programstep. If failed, go to wait state
          //_log.print(_buf);
          parclib::trimFront(_buf);
          parclib::trimBack(_buf, BUFLEN);
          parclib::squeeze(_buf);
          uint8_t numSubStr;
          char* subStrs[6] = { 0 };
          parclib::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          if (numSubStr == 5) {
            PinRegData pin;
            pin.code3 = subStrs[0][0] == '1' ? 1 : 0;
            pin.code2 = subStrs[1][0] == '1' ? 1 : 0;
            pin.code1 = subStrs[2][0] == '1' ? 1 : 0;
            pin.code0 = subStrs[3][0] == '1' ? 1 : 0;
            pin.retries = atoi(subStrs[4]);

            Ao_t::_messages.fromTerminalToKeypadQueue.push(pin.raw);
          }
          else {
            _serial.println(F(" This ain't dull, bye."));
            return true;
          }
          _itBuf = 0;
          memset(_buf, 0, BUFLEN);
        }
        else if (_itBuf < BUFLEN) {
          // omit preceding spaces to save the buffer for the data.
          if (_itBuf > 0 || ch != ' ') {
            _buf[_itBuf] = ch;
            _itBuf++;
          }
        }
      }
    }

    bool stateReadingProgramStepsOnEntry() {
      while (_serial.available()) {
        char ch = _serial.read();
        _serial.print(ch);
        if (ch == '}') {
          _serial.println(F(", thank you."));
          return true;
          // send unblocked to KeypadAo
        }
        else if (ch == ';') {
          // create programstep. If failed, go to wait state
          parclib::trimFront(_buf);
          parclib::trimBack(_buf, BUFLEN);
          parclib::squeeze(_buf);
          // Debug: _serial.println(_buf);

          uint8_t numSubStr;
          char* subStrs[6] = { 0 };
          parclib::split(_buf, BUFLEN, ' ', subStrs, &numSubStr);

          ProgramStep<TLOGGERFAC>* progStep = 0;

          if (CmdComparator<PsType::Wait>()(subStrs[0])) {
            progStep = createProgramStepWait(subStrs[1]);
          }
          else if (CmdComparator<PsType::BleKeycode>()(subStrs[0])) {
            progStep = createProgramStepBleKeyboardCode(subStrs, numSubStr);
          }
          else if (CmdComparator<PsType::BleText>()(subStrs[0])) {
            progStep = createProgramStepBleKeyboardText(subStrs[1]);
          }
          else if (CmdComparator<PsType::BleControlkey>()(subStrs[0])) {
            progStep = createProgramStepBleControlKey(subStrs, numSubStr);
          }
          else if (CmdComparator<PsType::UsbKeycode>()(subStrs[0])) {
            progStep = createProgramStepUsbKeyboardCode(subStrs, numSubStr);
          }
          else if (CmdComparator<PsType::UsbText>()(subStrs[0])) {
            progStep = createProgramStepUsbKeyboardText(subStrs[1]);
          }

          if (progStep != 0) {
            uint8_t progIdx = _keyPadInput.programIndex();
            _programs[progIdx].appendStep(progStep);
            Ao_t::_messages.fromTerminalToServiceMonitorQueue.push(ProgramChangedRegData(1));
          }
          else {
            _serial.println(F(" Unknown command. This ain't dull, bye."));
            // Remove the already processed program steps from the program.
            uint8_t progIdx = _keyPadInput.programIndex();
            _programs[progIdx].dispose();
            return true;
          }

          _itBuf = 0;
          memset(_buf, 0, BUFLEN);
        }
        else if (_itBuf < BUFLEN) {
          // omit preceding spaces to save the buffer for the data.
          if (_itBuf > 0 || ch != ' ') {
            _buf[_itBuf] = ch;
            _itBuf++;
          }
        }
        else {
          _serial.println(F(" Command string too long. This ain't dull, bye."));
          // Remove the already processed program steps from the program.
          uint8_t progIdx = _keyPadInput.programIndex();
          _programs[progIdx].dispose();
          return true;
        }
      }
    }

    static ProgramStep<TLOGGERFAC>* createProgramStepWait(const char* delay) {
      uint16_t waitMs = atoi(delay);
      // Debug: _log.print(F("Delay: ")); _log.println(waitMs);

      auto log = TLOGGERFAC::create();
      log->println(F("Create Wait_t"));
      return new Wait_t(waitMs);
    }

    static ProgramStep<TLOGGERFAC>* createProgramStepBleKeyboardCode(char* subStrs[], uint8_t numSubStr) {
      KeyCode keyCode;
      uint8_t repetitions = 0;
      for (int n = 1; n < numSubStr; n++) {
        keyCode.ctrl |= strcmp(subStrs[n], "<Ctrl>") == 0;
        keyCode.shift |= strcmp(subStrs[n], "<Shift>") == 0;
        keyCode.alt |= strcmp(subStrs[n], "<Alt>") == 0;
        keyCode.win |= strcmp(subStrs[n], "<Win>") == 0;

        if (strcmp2("-r", subStrs[n][0], subStrs[n][1])) {
          repetitions = atoi(&subStrs[n][2]);
        }
      }

      auto code = subStrs[numSubStr - 1];
      keyCode.hexCode = strtol(subStrs[numSubStr - 1], 0, KNOWNKEYCODES::BleRadix);
      if (keyCode.hexCode == 0) {
        keyCode.hexCode = symbolToBleKeyode(code);
      }
      auto log = TLOGGERFAC::create();
      //log.print("ProgramStepBleKeyboardCode "); _log.print(hexCode, HEX);
      if (repetitions == 0) {
        log->println(F("Create BleKeycode_t"));
        return new BleKeycode_t(keyCode);
      }
      log->println(F("Create BleKeycodeRepeated_t"));
      return new BleKeycodeRepeated_t(keyCode, repetitions);
    }

    static ProgramStep<TLOGGERFAC>* createProgramStepBleKeyboardText(char* text) {
      auto log = TLOGGERFAC::create();
      //_log.print("TEXT: "); _log.println(text);
      log->println(F("Create BleText_t"));
      return new BleText_t(text);
    }

    static ProgramStep<TLOGGERFAC>* createProgramStepBleControlKey(char* subStrs[], uint8_t numSubStr) {
      auto log = TLOGGERFAC::create();
      // _log.print("Control Key: "); _log.print(subStrs[1]);

      if (strcmp(subStrs[1], "Volume+") == 0) {
        return new BleControlkey_t("0xE9");
      }
      if (strcmp(subStrs[1], "Volume-") == 0) {
        return new BleControlkey_t("0xEA");
      }

      log->println(F("Create BleControlkey_t"));
      return new BleControlkey_t(subStrs[1]);
    }

    static ProgramStep<TLOGGERFAC>* createProgramStepUsbKeyboardCode(char* subStrs[], uint8_t numSubStr) {
      KeyCode keyCode;
      uint8_t repetitions = 0;
      char second = 0;
      for (int n = 1; n < numSubStr; n++) {
        keyCode.ctrl |= strcmp(subStrs[n], "<Ctrl>") == 0;
        keyCode.shift |= strcmp(subStrs[n], "<Shift>") == 0;
        keyCode.alt |= strcmp(subStrs[n], "<Alt>") == 0;
        keyCode.win |= strcmp(subStrs[n], "<Win>") == 0;

        if (strcmp2("-r", subStrs[n][0], subStrs[n][1])) {
          repetitions = atoi(&subStrs[n][2]);
        }
      }

      auto code = subStrs[numSubStr - 1];
      auto log = TLOGGERFAC::create();
      //Debug: _log.print("createProgramStepUsbKeyboardCode "); _log.println(code);

      if (strlen(code) == 3) {
        if (code[0] == '\'' && code[2] == '\'') {
          keyCode.hexCode = code[1];
        }
        else if ( repetitions == 0) {
          keyCode.hexCode = code[0];
          second = code[2];
        }
      }
      else {
        keyCode.hexCode = strtol(code, 0, KNOWNKEYCODES::UsbRadix);
      }
      //Debug: _log.print("createProgramStepUsbKeyboardCode "); _log.println(keyCode.hexCode, HEX);
      if (keyCode.hexCode == 0) {
        keyCode.hexCode = symbolToUsbKeyode(code);
      }
      //Debug: _log.print("createProgramStepUsbKeyboardCode "); _log.println(keyCode.hexCode, HEX);
      if (second != 0) {
        log->println(F("Create UsbKeycodes_t"));
        return new UsbKeycodes_t(keyCode, second);
      }
      if (repetitions == 0) {
        log->println(F("Create UsbKeycode_t"));
        return new UsbKeycode_t(keyCode);
      }
      log->println(F("Create UsbKeycodeRepeated_t"));
      return new UsbKeycodeRepeated_t(keyCode, repetitions);
    }

    static ProgramStep<TLOGGERFAC>* createProgramStepUsbKeyboardText(char* text) {
      auto log = TLOGGERFAC::create();
      // log.print("TEXT: "); _log.println(text);
      log->println(F("Create UsbText_t"));
      return new UsbText_t(text);
    }

    static uint8_t symbolToUsbKeyode(const char* code) {
      if (strcmp(code, "<Del>") == 0) {
        return KNOWNKEYCODES::UsbKeyCodeDel;
      }
      if (strcmp(code, "<Tab>") == 0) {
        return KNOWNKEYCODES::UsbKeyCodeTab;
      }
      if (strcmp(code, "<Enter>") == 0) {
        return KNOWNKEYCODES::UsbKeyCodeEnter;
      }
      if (strcmp(code, "<Space>") == 0) {
        return KNOWNKEYCODES::UsbKeyCodeSpace;
      }
      return 0;
    }

    static uint8_t symbolToBleKeyode(const char* code) {
      if (strcmp(code, "<Del>") == 0) {
        return KNOWNKEYCODES::BleKeyCodeDel;
      }
      if (strcmp(code, "<Tab>") == 0) {
        return KNOWNKEYCODES::BleKeyCodeTab;
      }
      if (strcmp(code, "<Enter>") == 0) {
        return KNOWNKEYCODES::BleKeyCodeEnter;
      }
      if (strcmp(code, "<Space>") == 0) {
        return KNOWNKEYCODES::BleKeyCodeSpace;
      }
      return 0;
    }

  private:
    using Ao_t = Ao<TerminalAo<PROGSTEPFACTORY, TSERIAL, TLOGGERFAC, THIDBLEFAC, THIDUSBFAC, TPROGRAM, TSYSTEMHWFAC, KNOWNKEYCODES, BUFLEN>>;
    using Wait_t = typename TypeAt<PROGSTEPFACTORY, PsType::Wait>::Result;
    using BleKeycode_t = typename TypeAt<PROGSTEPFACTORY, PsType::BleKeycode>::Result;
    using BleKeycodeRepeated_t = typename TypeAt<PROGSTEPFACTORY, PsType::BleKeycodeRepeated>::Result;
    using BleText_t = typename TypeAt<PROGSTEPFACTORY, PsType::BleText>::Result;
    using BleControlkey_t = typename TypeAt<PROGSTEPFACTORY, PsType::BleControlkey>::Result;
    using UsbKeycode_t = typename TypeAt<PROGSTEPFACTORY, PsType::UsbKeycode>::Result;
    using UsbKeycodeRepeated_t = typename TypeAt<PROGSTEPFACTORY, PsType::UsbKeycodeRepeated>::Result;
    using UsbKeycodes_t = typename TypeAt<PROGSTEPFACTORY, PsType::UsbKeycodes>::Result;
    using UsbText_t = typename TypeAt<PROGSTEPFACTORY, PsType::UsbText>::Result;

    template<typename T>
    struct Identity { typedef T type; };

    struct Idle : public tsmlib::BasicState<Idle, StatePolicy, false, false, true>, public tsmlib::SingletonCreator<Idle> {

      bool gotProgramCode = false;

      template<class Event> void doit(const Event& ev) { doit(ev, Identity<Event>()); }
      template<class Event> void doit(const Event&, Identity<Event>) {}
      void doit(const Trigger::Timeout& ev, Identity<Trigger::Timeout>) {

        auto ao = static_cast<TerminalAo*>(ev.owner);
        gotProgramCode = ao->stateIdleDo();
      //  auto serial = &ao->_serial;
      //  auto programs = ao->_programs;
      //  auto itBuf = ao->_itBuf;
      //  auto buf = ao->_buf;

      //  char ch = 0;
      //  currentChar = 0;
      //  while (serial->available() && currentChar == 0) {
      //    ch = serial->read();
      //    currentChar = ch;

      //    if (ch == '{') {
      //      serial->print(F("> {"));

      //      itBuf = 0;
      //      memset(buf, 0, BUFLEN);
      //      ch = 0;

      //      break;
      //    }
      //    else if (ch == '?') {
      //      serial->println(F("Programmed:"));
      //      for (int m = 0; m < 4; m++) {
      //        for (int n = 1; n < 6; n++) {
      //          KeypadRegData input(m, n);
      //          serial->print(input.mode); serial->print(F(" ")); serial->print(char(input.button - 1 + 'A')); serial->print(F(" : "));
      //          programs[input.programIndex()].hasSteps() ? serial->println(F("Yes")) : serial->println(F("No"));
      //        }
      //      }
      //      auto sysHw = TSYSTEMHWFAC::create();
      //      serial->print(F("Remaining bytes: ")); serial->println(sysHw->freeMemory());
      //      break;
      //    }
      //    else if (isprint(ch)) {
      //      serial->print(ch);
      //      currentChar = 0;
      //    }
      //  }
      //  if (ch != 0 /*&& ch != 0b1101 && ch != 0b1010*/) {
      //    // YAT sends 0b1101 and 0b1010, PuTTY sends 0b1101 and powershell sends 0b1010
      //    // Debug: Serial.println("Enter pressed");
      //    // Debug: Serial.println(); //eol of repeated command
      //    serial->println(F("Start defining your dull programs."));
      //  }
      }
    };

    struct ReadingProgramCode : public tsmlib::BasicState<ReadingProgramCode, StatePolicy, true>, public tsmlib::SingletonCreator<ReadingProgramCode> {

      template<class Event> void entry(const Event& ev) { entry(ev, Identity<Event>()); }
      template<class Event> void entry(const Event&, Identity<Event>) {}
      void entry(const Trigger::Timeout& ev, Identity< Trigger::Timeout>) {
        auto ao = static_cast<TerminalAo*>(ev.owner);
        ao->stateReadingProgramCodeEntry();
      }
    };

    struct ReadingProgramSteps : public tsmlib::BasicState<ReadingProgramSteps, StatePolicy, true>, public tsmlib::SingletonCreator<ReadingProgramSteps> {

      bool readingDone = false;

      template<class Event> void entry(const Event& ev) { entry(ev, Identity<Event>()); }
      template<class Event> void entry(const Event&, Identity<Event>) {}
      void entry(const Trigger::Timeout& ev, Identity< Trigger::Timeout>) {

        auto ao = static_cast<TerminalAo*>(ev.owner);

        readingDone = ao->stateReadingProgramStepsOnEntry();

        //auto serial = &ao->_serial;
        //auto programs = ao->_programs;
        //auto itBuf = ao->_itBuf;
        //auto buf = ao->_buf;
        //auto keyPadState = &ao->_keyPadInput;
        //auto messages = ao->_messages;

        //readingDone = false;

        //while (serial->available()) {
        //  char ch = serial->read();
        //  serial->print(ch);
        //  if (ch == '}') {
        //    serial->println(F(", thank you."));
        //    readingDone = true;
        //    break;
        //    // send unblocked to KeypadAo
        //  }
        //  else if (ch == ';') {
        //    // create programstep. If failed, go to wait state
        //    parclib::trimFront(buf);
        //    parclib::trimBack(buf, BUFLEN);
        //    parclib::squeeze(buf);
        //    // Debug: _serial.println(_buf);

        //    uint8_t numSubStr;
        //    char* subStrs[6] = { 0 };
        //    parclib::split(buf, BUFLEN, ' ', subStrs, &numSubStr);

        //    ProgramStep<TLOGGERFAC>* progStep = 0;

        //    if (CmdComparator<PsType::Wait>()(subStrs[0])) {
        //      progStep = createProgramStepWait(subStrs[1]);
        //    }
        //    else if (CmdComparator<PsType::BleKeycode>()(subStrs[0])) {
        //      progStep = createProgramStepBleKeyboardCode(subStrs, numSubStr);
        //    }
        //    else if (CmdComparator<PsType::BleText>()(subStrs[0])) {
        //      progStep = createProgramStepBleKeyboardText(subStrs[1]);
        //    }
        //    else if (CmdComparator<PsType::BleControlkey>()(subStrs[0])) {
        //      progStep = createProgramStepBleControlKey(subStrs, numSubStr);
        //    }
        //    else if (CmdComparator<PsType::UsbKeycode>()(subStrs[0])) {
        //      progStep = createProgramStepUsbKeyboardCode(subStrs, numSubStr);
        //    }
        //    else if (CmdComparator<PsType::UsbText>()(subStrs[0])) {
        //      progStep = createProgramStepUsbKeyboardText(subStrs[1]);
        //    }

        //    if (progStep != 0) {
        //      uint8_t progIdx = keyPadState->programIndex();
        //      programs[progIdx].appendStep(progStep);
        //      messages.fromTerminalToServiceMonitorQueue.push(ProgramChangedRegData(1));
        //    }
        //    else {
        //      serial->println(F(" Unknown command. This ain't dull, bye."));
        //      // Remove the already processed program steps from the program.
        //      uint8_t progIdx = keyPadState->programIndex();
        //      programs[progIdx].dispose();
        //      readingDone = true;
        //      break;
        //    }

        //    itBuf = 0;
        //    memset(buf, 0, BUFLEN);
        //  }
        //  else if (itBuf < BUFLEN) {
        //    // omit preceding spaces to save the buffer for the data.
        //    if (itBuf > 0 || ch != ' ') {
        //      buf[itBuf] = ch;
        //      itBuf++;
        //    }
        //  }
        //  else {
        //    serial->println(F(" Command string too long. This ain't dull, bye."));
        //    // Remove the already processed program steps from the program.
        //    uint8_t progIdx = keyPadState->programIndex();
        //    programs[progIdx].dispose();
        //    readingDone = true;
        //    break;
        //  }
        //}
      }
    };

    struct ReadingPin : public tsmlib::BasicState<ReadingPin, StatePolicy, true>, public tsmlib::SingletonCreator<ReadingPin> {

      bool readingDone = false;

      template<class Event> void entry(const Event& ev) { entry(ev, Identity<Event>()); }
      template<class Event> void entry(const Event&, Identity<Event>) {}
      void entry(const Trigger::Timeout& ev, Identity< Trigger::Timeout>) {

        auto ao = static_cast<TerminalAo*>(ev.owner);
        readingDone = ao->stateReadingPinEntry();

        //auto serial = &ao->_serial;
        //auto programs = ao->_programs;
        //auto itBuf = ao->_itBuf;
        //auto buf = ao->_buf;
        //auto keyPadState = &ao->_keyPadInput;
        //auto messages = &ao->_messages;

        //while (serial->available()) {
        //  char ch = serial->read();
        //  serial->print(ch);
        //  if (ch == '}') {
        //    serial->println(F(", thank you."));
        //    readingDone = true;
        //    break;
        //    // send unblocked to KeypadAo
        //  }
        //  else if (ch == ';') {
        //    // create programstep. If failed, go to wait state
        //    //_log.print(_buf);
        //    parclib::trimFront(buf);
        //    parclib::trimBack(buf, BUFLEN);
        //    parclib::squeeze(buf);
        //    uint8_t numSubStr;
        //    char* subStrs[6] = { 0 };
        //    parclib::split(buf, BUFLEN, ' ', subStrs, &numSubStr);

        //    if (numSubStr == 5) {
        //      PinRegData pin;
        //      pin.code3 = subStrs[0][0] == '1' ? 1 : 0;
        //      pin.code2 = subStrs[1][0] == '1' ? 1 : 0;
        //      pin.code1 = subStrs[2][0] == '1' ? 1 : 0;
        //      pin.code0 = subStrs[3][0] == '1' ? 1 : 0;
        //      pin.retries = atoi(subStrs[4]);

        //      messages->fromTerminalToKeypadQueue.push(pin.raw);
        //    }
        //    else {
        //      serial->println(F(" This ain't dull, bye."));
        //      readingDone = true;
        //      break;
        //    }
        //    itBuf = 0;
        //    memset(buf, 0, BUFLEN);
        //  }
        //  else if (itBuf < BUFLEN) {
        //    // omit preceding spaces to save the buffer for the data.
        //    if (itBuf > 0 || ch != ' ') {
        //      buf[itBuf] = ch;
        //      itBuf++;
        //    }
        //  }
        //}
      }
    };

    struct ToReadingProgramCodeFromIdleGuard {
      bool eval(const Idle& idle, const Trigger::Timeout& ev) {
        return idle.gotProgramCode;
      }
    };

    struct ToReadingProgramStepsFromReadingProgramCodeGuard
    {
      bool eval(const ReadingProgramCode&, const Trigger::Timeout& ev) {
        auto ao = static_cast<TerminalAo*>(ev.owner);
        auto idx = ao->_keyPadInput.programIndex();

        return 0 <= idx && idx < NumberOfPrograms;
      }
    };

    struct ToReadingPinFromReadingProgramCodeGuard
    {
      bool eval(const ReadingProgramCode&, const Trigger::Timeout& ev) {
        auto ao = static_cast<TerminalAo*>(ev.owner);

        return ao->_keyPadInput.isPin;
      }
    };

    struct ToIdleFromReadingProgramStepsGuard
    {
      bool eval(const ReadingProgramSteps& readingProgramSteps, const Trigger::Timeout&) {
        return readingProgramSteps.readingDone;
      }
    };

    struct ToIdleFromReadingPinGuard
    {
      bool eval(const ReadingPin& readingPin, const Trigger::Timeout&) {
        return readingPin.readingDone;
      }
    };

    using ToReadingProgramCodeFromIdle = tsmlib::ChoiceTransition<Trigger::Timeout, ReadingProgramCode, Idle, Idle, ToReadingProgramCodeFromIdleGuard, tsmlib::NoAction>;
    using ToReadingPinOrReadingProgramStepsFromReadingProgramCode = tsmlib::Choice2Transition<Trigger::Timeout, ReadingPin, ReadingProgramSteps, Idle, ReadingProgramCode, ToReadingPinFromReadingProgramCodeGuard, ToReadingProgramStepsFromReadingProgramCodeGuard, tsmlib::NoAction>;
    using ToIdleFromReadingProgramSteps = tsmlib::Transition<Trigger::Timeout, Idle, ReadingProgramSteps, ToIdleFromReadingProgramStepsGuard, tsmlib::NoAction>;
    using ToIdleFromReadingPin = tsmlib::Transition<Trigger::Timeout, Idle, ReadingPin, ToIdleFromReadingPinGuard, tsmlib::NoAction>;

    using Transitions =
      tsmlib::Typelist<ToReadingProgramCodeFromIdle,
      tsmlib::Typelist<ToReadingPinOrReadingProgramStepsFromReadingProgramCode,
      tsmlib::Typelist<ToIdleFromReadingProgramSteps,
      tsmlib::Typelist<ToIdleFromReadingPin,
      tsmlib::NullType>>>>;

    using InitTransition = tsmlib::InitialTransition<Idle, tsmlib::NoAction>;
    using Sm = tsmlib::Statemachine<Transitions, InitTransition>;

    Sm _statemachine;

    TSERIAL& _serial;
    TPROGRAM* _programs;

    char _buf[BUFLEN] = { 0 };
    uint8_t _itBuf = 0;

    KeypadRegData _keyPadInput;
    MessageData_t _pinDefinedMsg = 0;
    BitTimer<0> _timer;
};

}
