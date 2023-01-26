/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2011 Nexenta Systems, Inc.  All rights reserved.
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include "libm_protos.h"

/*
 * Let y[j] = _TBL_atan[2j], atan_y[j] = _TBL_atan[2j+1], j = 0, 1, ..., 95.
 * {y[j], 0 <= j < 96} is a set of break points in (-1/8, 8) chosen so that
 * the high part of y[j] is very close to 0x3fc08000 + (j << 16),
 * and atan_y[j] = atan(y[j]) rounded has relative error bounded by 2^-60.
 *
 * -- K.C. Ng, 10/17/2004
 */

const double _TBL_atan[] = {
1.28906287871928065814e-01,  1.28199318484201185697e-01,
1.36718905591866640714e-01,  1.35876480966603985223e-01,
1.44531257606217988787e-01,  1.43537301152401930437e-01,
1.52343679482641575218e-01,  1.51181262880709432750e-01,
1.60156177403962790562e-01,  1.58807537535115006477e-01,
1.67968772982362929413e-01,  1.66415323534856884891e-01,
1.75781211596017922227e-01,  1.74003563682464612583e-01,
1.83593807762862160082e-01,  1.81571767039387044207e-01,
1.91406205589629646591e-01,  1.89118806085245338977e-01,
1.99218440148815872925e-01,  1.96643947167121080355e-01,
2.07031180070658488157e-01,  2.04147078126891479144e-01,
2.14843557086546094181e-01,  2.11626624363759674452e-01,
2.22656308649619494311e-01,  2.19082566659412503185e-01,
2.30468759807905931858e-01,  2.26513550670145669130e-01,
2.38281413377399470255e-01,  2.33919360814280885563e-01,
2.46093763828156536499e-01,  2.41298839969374956382e-01,
2.57812599322508773092e-01,  2.52318074018685223336e-01,
2.73437443946477509726e-01,  2.66912935433335718471e-01,
2.89062532292519769328e-01,  2.81392462451501401688e-01,
3.04687577351389293767e-01,  2.95751756530947318424e-01,
3.20312405527377053183e-01,  3.09986305565206343715e-01,
3.35937715576634265968e-01,  3.24092664204967739749e-01,
3.51562621385942464247e-01,  3.38066230870244233131e-01,
3.67187719833070636000e-01,  3.51904019130060419229e-01,
3.82812538440931826589e-01,  3.65602365234580339859e-01,
3.98437724467857745658e-01,  3.79158862748537828224e-01,
4.14062683287296784407e-01,  3.92570291474021892952e-01,
4.29687654458357937148e-01,  4.05834423459965343284e-01,
4.45312642848883721847e-01,  4.18949086342842669239e-01,
4.60937644536906665493e-01,  4.31912354681638355203e-01,
4.76563149131543906112e-01,  4.44722952952162131623e-01,
4.92187842452541601812e-01,  4.57378374341803173309e-01,
5.15624825518001039804e-01,  4.76069192487019954285e-01,
5.46874516057966109095e-01,  5.00440440618262982753e-01,
5.78125566624434150675e-01,  5.24180053466007933594e-01,
6.09375102172641347487e-01,  5.47284455493244337276e-01,
6.40624936950189516338e-01,  5.69756408779493739303e-01,
6.71875248719545625775e-01,  5.91599881698465779323e-01,
7.03124988865964306584e-01,  6.12820194714659649549e-01,
7.34376295967088421612e-01,  6.33426724884753156175e-01,
7.65624929092156736310e-01,  6.53426296477277901431e-01,
7.96874196003358736817e-01,  6.72832055855442590087e-01,
8.28125565205639735389e-01,  6.91656957129326954714e-01,
8.59375453355927021448e-01,  7.09911879233846576653e-01,
8.90625694745052709500e-01,  7.27611720056701827275e-01,
9.21875110259870345075e-01,  7.44770185320721367361e-01,
9.53125042657123722201e-01,  7.61402792157321428590e-01,
9.84374765277631902372e-01,  7.77524191164056688308e-01,
1.03126494373528343473e+00,  8.00788807142382097481e-01,
1.09374968909110092952e+00,  8.30144253291031475328e-01,
1.15625019152505204012e+00,  8.57735575892430546219e-01,
1.21874985186151341132e+00,  8.83672057048812575886e-01,
1.28124876006842702836e+00,  9.08066349515326720621e-01,
1.34375006271148444981e+00,  9.31026566320014126177e-01,
1.40627222899692072566e+00,  9.52659566341466756967e-01,
1.46874957658300542285e+00,  9.73037801091363618866e-01,
1.53124999999999555911e+00,  9.92272112377190040888e-01,
1.59375089676214143353e+00,  1.01043670320979472876e+00,
1.65624949800269094524e+00,  1.02760661639661776690e+00,
1.71874946971376685312e+00,  1.04385296549501305208e+00,
1.78125111924655166185e+00,  1.05924046784549474864e+00,
1.84374921332370989013e+00,  1.07382754310190620117e+00,
1.90625055239083862624e+00,  1.08767078118685489585e+00,
1.96874992734227549640e+00,  1.10081967347672460278e+00,
2.06250046973591683042e+00,  1.11934332464931074469e+00,
2.18749905173933534286e+00,  1.14201813543610697366e+00,
2.31249933788800232648e+00,  1.16264711873167669864e+00,
2.43749855191054187742e+00,  1.18147939634549814514e+00,
2.56251104936881235474e+00,  1.19873002825057639598e+00,
2.68750036758144528193e+00,  1.21457671610223272296e+00,
2.81249907059852954916e+00,  1.22918073183895870670e+00,
2.93749583903062294610e+00,  1.24267599964591468620e+00,
3.06250108260464948273e+00,  1.25518076906426045980e+00,
3.18750016629930410517e+00,  1.26679540235591403530e+00,
3.31250071362610132297e+00,  1.27760948984166233799e+00,
3.43749999999999333866e+00,  1.28770054149540058575e+00,
3.56249877589327157423e+00,  1.29713691630583838332e+00,
3.68750696071718842006e+00,  1.30597947372626776996e+00,
3.81250023149192607264e+00,  1.31427972905173717777e+00,
3.93749827850909683846e+00,  1.32208623339324304879e+00,
4.12500187917697846984e+00,  1.33296050364557672196e+00,
4.37499759905160701123e+00,  1.34608503917096200553e+00,
4.62500066729278191957e+00,  1.35785800701782477518e+00,
4.87499852385410648026e+00,  1.36847463881194641999e+00,
5.12499918742110072145e+00,  1.37809553833018583191e+00,
5.37500000000004529710e+00,  1.38685287025772296943e+00,
5.62499999999991828759e+00,  1.39485670134236627860e+00,
5.87499417854096694924e+00,  1.40219922327269230777e+00,
6.12500000000013233858e+00,  1.40895889555647713109e+00,
6.37499999999991828759e+00,  1.41520149881786494461e+00,
6.62499933107761584949e+00,  1.42098385532083781868e+00,
6.87500431528593747288e+00,  1.42635483782722261026e+00,
7.12499228632883863099e+00,  1.43135612069194451124e+00,
7.37499257154547205317e+00,  1.43602490820671135907e+00,
7.62499911873607416624e+00,  1.44039300400460135165e+00,
7.87500000000018918200e+00,  1.44448820973165936721e+00,
};
