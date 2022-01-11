import flask
from flask.views import MethodView
from markdown import markdown
from pygments.formatters.html import HtmlFormatter
from flask_cors import CORS
from vagas import get_vagas, vagas_table_template
import serial


app = flask.Flask(__name__)
CORS(app)
app.config["DEBUG"] = True


@app.route('/', methods=['GET', 'POST'])
def home():
    readme = open("README.md", 'r', encoding="utf-8")
    md_template = markdown(readme.read(), extensions=["fenced_code"])
    formatter = HtmlFormatter(style="emacs", full=True, cssclass="codehilite")
    css_string = formatter.get_style_defs()
    md_css_string = "<style>" + css_string + "</style>"
    md_template = md_css_string + md_template
    return md_template


class VagasView(MethodView):

    def get(self):
        # Seleciona qual porta serial irá ler
        arduino = serial.Serial(
            port='COM7',
            baudrate=9600,
            timeout=.1
        )
        # Lê os dados
        result = get_vagas(arduino)
        # Processa os dados e transforma no
        # template da tabela
        table = vagas_table_template(result)
        return table


vagas_view = VagasView.as_view('vagas_view')
app.add_url_rule(
    '/vagas/',
    view_func=vagas_view,
    methods=['GET']
)


if __name__ == "__main__":
    app.run()
