import android.app.Dialog
import android.graphics.Color
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import androidx.appcompat.app.AlertDialog
import androidx.fragment.app.DialogFragment
import com.example.orellanaproject2.R
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.google.firebase.database.*

class GraficoFragment(private val tipo: String) : DialogFragment() {


    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        val view = LayoutInflater.from(requireContext())
            .inflate(R.layout.fragment_grafico, null)

        val chart = view.findViewById<LineChart>(R.id.lineChart)
        chart.setBackgroundColor(Color.WHITE)
        chart.axisLeft.textColor = Color.BLACK
        chart.axisRight.textColor = Color.BLACK
        chart.xAxis.textColor = Color.BLACK
        chart.legend.textColor = Color.BLACK
        chart.description.textColor = Color.BLACK

        val database = FirebaseDatabase.getInstance()
        val ref = database.getReference("Sensores")

        val entries = mutableListOf<Entry>()

        ref.addListenerForSingleValueEvent(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                var index = 0f

                // Aqui pegamos os dados direto (sem histórico, então será só 1 valor)
                val valor = when (tipo) {
                    "Temperatura" -> snapshot.child("Temperatura").getValue(Float::class.java)
                    "Umidade" -> snapshot.child("Umidade").getValue(Float::class.java)
                    "Luminosidade" -> snapshot.child("Luminosidade").getValue(Float::class.java)
                    "Umidade do Solo" -> snapshot.child("SensorDeUmidadeDoSolo").getValue(Float::class.java)
                    else -> null
                }

                if (valor != null) {
                    entries.add(Entry(index, valor))
                }

                val dataSet = LineDataSet(entries, tipo).apply {
                    color = when (tipo) {
                        "Temperatura" -> Color.RED
                        "Umidade" -> Color.BLUE
                        "Luminosidade" -> Color.YELLOW
                        "Umidade do Solo" -> Color.GREEN
                        else -> Color.GRAY
                    }
                    valueTextColor = Color.BLACK
                    lineWidth = 2f
                    circleRadius = 4f
                    setDrawValues(true)
                }

                chart.data = LineData(dataSet)
                chart.invalidate()
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Erro ao ler dados", error.toException())
            }
        })

        return AlertDialog.Builder(requireContext())
            .setTitle("Gráfico de $tipo")
            .setView(view)
            .setPositiveButton("Fechar") { dialog, _ -> dialog.dismiss() }
            .create()
    }
}