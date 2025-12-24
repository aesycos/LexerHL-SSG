#!/bin/python3

import json
from pathlib import Path
import subprocess
import sys
import shutil

PROJECT_ROOT = Path(__file__).resolve().parents[1]
CODE_ROOT = PROJECT_ROOT / "content" / "code"
TEMPLATE_ROOT = PROJECT_ROOT / "content" / "templates"
OUT_ROOT = PROJECT_ROOT / "content" / "output"

print(PROJECT_ROOT)
print(CODE_ROOT)

def generateCodeSnippet( src: Path, dest: Path | None = None ) :
	print(src)
	print(dest)

	if dest is None :
		dest = src.parent / "snippets" / src.name

	else :
		if dest.is_dir() :
			dest = dest / src.name

	print(f"Creating folder if not exist{dest.parent}")
	dest.parent.mkdir(parents=True, exist_ok=True)

	src = CODE_ROOT / src.parent.name / src.name

	print(f"src{src}")

	subprocess.run(
		[ "./sitegen-src/build/release/sitegen", src ],
		stdout = dest.open("w"),
		check = True
	)

	return



def renderSection( section: dict, snippetsDir: Path ) -> str :
	html = []

	if section["type"] == "text" :
		return f"<p>{section['content']}</p>"

	elif section["type"] == "code" :
		src = (PROJECT_ROOT / "code" / snippetsDir.parent / section["file"]).resolve()
		snippet = snippetsDir / ( src.name + ".html" )

		generateCodeSnippet( src, snippet )

		return f"""<pre><code data-type="{section['language']}">
{snippet.read_text()}
</code></pre>"""

	return ""



def flattenSections( sections: list ) -> list :
	blocks = []

	for section in sections :
		for subsection in section["subsections"] :
			for _, items in subsection.items() :
				blocks.extend( items )

	return blocks



def buildArticle( page: dict, pageDir: Path ) -> str :
	snippetsDir = pageDir / "snippets"

	article = [f"<article><h3>{page['title']}</h3>"]

	for block in flattenSections( page["sections"] ) :
		article.append( renderSection( block, snippetsDir ) )

	article.append( "</article>" )

	return "\n".join(article)



def injectIntoTemplate( article: str, template: str, outputPath: Path) :
	template = (TEMPLATE_ROOT / template).read_text()
	finalPage = template.replace("{{content}}", article)

	outputPath.write_text(finalPage)




def main( path: Path, files: list | None = None ) :
	if not path.is_dir() :
		print("Bad path supplied.")
		return 1

	if not files :
		print("No files to parse")
		return 1

	for file in files :
		pageJSON = json.loads( (path / file).read_text() )

		article = buildArticle( pageJSON, path )
		outputHTML = (path / f"{pageJSON['slug']}.html")

		for key in pageJSON :
			print(key)

		injectIntoTemplate( article, f"{pageJSON['template']}", outputHTML )

		print(f"Generated {outputHTML}")

		outputFolder = OUT_ROOT / pageJSON['slug']
		cssFolder = outputFolder / "css"
		scriptsFolder = outputFolder / "scripts"
		htmlFolder = outputFolder / "html"
		print(f"Creating folder if not exist{outputFolder}")
		print(f"Creating folder if not exist{cssFolder}")
		print(f"Creating folder if not exist{scriptsFolder}")
		print(f"Creating folder if not exist{htmlFolder}")
		print(f"Copying files to {outputFolder}....")
		
		for folder in [ outputFolder, cssFolder, scriptsFolder, htmlFolder] :
			folder.mkdir(parents=True, exist_ok=True)

		for style in pageJSON['styles'] :
			print(f"Copying {style}.css to {cssFolder}....")
			shutil.copyfile( PROJECT_ROOT / "theme/css" / f"{style}.css", 
							 cssFolder / f"{style}.css"
			)

		for script in pageJSON['scripts'] :
			shutil.copyfile( PROJECT_ROOT / "theme/js" / f"{script}.js", 
							 scriptsFolder / f"{script}.js"
			)

		shutil.copyfile( outputHTML, htmlFolder / outputHTML.name )


	return 0

if __name__ == "__main__" :
	if len(sys.argv) != 2:
		print("Usage: assemble.py <content>")
		sys.exit(1)

	if len(sys.argv) > 1:
		projectFolder = PROJECT_ROOT / "content/pages" / sys.argv[1]

		try :
			files = [ item.name for item in projectFolder.iterdir()
							if item.name.endswith(".json")
					]
		except:
			print("An exception occured trying to open {}".format(projectFolder))
			exit(1)
	else :
		print("No Project Directory Supplied!")
		exit(1)

	main( projectFolder, files )
	exit(0)