\ProvidesClass{report-UTC}

\LoadClass[a4paper,12pt,openright]{article}

\usepackage[utf8]{inputenc}
\usepackage{graphicx}
\usepackage{wrapfig}
\usepackage{caption}
\usepackage{bbold}
\usepackage[most]{tcolorbox}
\usepackage{array}
\usepackage{mwe}
\usepackage{titlepic}
\usepackage{dsfont}
\usepackage[toc,page]{appendix} 
\usepackage[normalem]{ulem}
\usepackage{subfiles}
\usepackage{listings}
\usepackage{lastpage}
\usepackage[english]{babel} % Package babel pour l'anglais
\usepackage[T1]{fontenc}    % Package pour les accentuations
\usepackage[utf8]{inputenc}  % Français
\usepackage{subfiles}
\usepackage{titling}
\usepackage{textcomp}
\usepackage{lmodern}        % Pour avoir de bonnes polices en pdf
\usepackage{graphicx}       % Indispensable pour les figures
\usepackage{amsmath}        % Environnement pour les maths, permet du mettre du texte dans les équations
\usepackage[headheight=61pt]{geometry}       % Utilisé pour les marges
\usepackage{multicol}		% Pour les colonnes
\usepackage{mathtools}  % Typographie pour les ensembles communs
\usepackage{amssymb}    % Typographie pour les ensembles communs
\usepackage{float}          % Pour bien placer les figures, scripts et tableaux
\usepackage{xspace}         % for including spaces at the end of latex macros
\usepackage{todonotes}
\usepackage{xcolor}
\usepackage{enumitem} %For custom itemize
\usepackage{tikz}			%Pour les figures et graphes
\usetikzlibrary{calc}		%Pour les figures et graphes
\usepackage[cache=false]{minted}        % Utilisé pour les scripts
\geometry{vmargin=3cm, textwidth=16cm} % Réglages des marges
\usepackage{fancyhdr}		% Pour l'entête et les pieds de page
\pagestyle{fancy}			% Pour l'entête et les pieds de page
\usepackage[pdfencoding=auto, psdextra]{hyperref}		% Pour les liens hypertext, sommaire et références
\usepackage{url}            % For quickly inserting links
\usepackage[final]{pdfpages} % Pour inclure des .pdf

\usepackage{fancyhdr}

\usepackage{multicol}
\usepackage{csquotes}
\usepackage[backend=biber]{biblatex}
\addbibresource{bibli.bib}
 
\renewcommand{\footrulewidth}{0.4pt}% default is 0pt

\pagestyle{fancy}
\fancyhf{}
\lhead{\includegraphics[height=1cm]{assets/logo_utc.jpg}}
\rhead{\Large{\textbf{PRDW} - \textit{A18}}}
\rfoot{Page \thepage\ / \pageref{LastPage}}

% CommandShell Definition
\newtcblisting{commandshell}{colback=black,colupper=white,colframe=black!75!black, listing only,listing options={breaklines=true, postbreak={\raisebox{0ex}[0ex][0ex]{\ensuremath{\color{green}\hookrightarrow\space}}},language=sh},
every listing line={\textcolor{green}{\small\ttfamily\bfseries \$> }}}


\newenvironment{myabstract}{%
\begin{quote} \begin{center}
 \bf}
{\end{center}
 \end{quote}}
\lstset{language=Python}