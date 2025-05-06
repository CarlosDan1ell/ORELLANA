package com.example.orellanaproject2

import android.app.Dialog
import android.os.Bundle
import android.view.LayoutInflater
import androidx.appcompat.app.AlertDialog
import androidx.fragment.app.DialogFragment
import com.github.mikephil.charting.data.*
import com.github.mikephil.charting.charts.LineChart
import com.example.orellanaproject2.R

class GraficoDialogFragment : DialogFragment() {

    override fun onCreateDialog(savedInstanceState: Bundle?): Dialog {
        val view = LayoutInflater.from(requireContext()).inflate(R.layout.fragment_dialog_grafico, null)
        val chart = view.findViewById<LineChart>(R.id.lineChart)
        val entries = listOf(
            Entry(1f, 10f),
            Entry(2f, 15f),
            Entry(3f, 8f),
            Entry(4f, 20f)
        )

        val dataSet = LineDataSet(entries, "Temperatura").apply {
            color = android.graphics.Color.BLUE
            valueTextColor = android.graphics.Color.BLACK
            lineWidth = 2f
            circleRadius = 4f
            setDrawValues(true)
        }

        chart.data = LineData(dataSet)
        chart.setBackgroundColor(android.graphics.Color.WHITE) // Fundo do gráfico
        chart.axisLeft.textColor = android.graphics.Color.BLACK
        chart.axisRight.textColor = android.graphics.Color.BLACK
        chart.xAxis.textColor = android.graphics.Color.BLACK
        chart.legend.textColor = android.graphics.Color.BLACK

        chart.invalidate()

        return AlertDialog.Builder(requireContext())
            .setTitle("Gráfico de Temperatura")
            .setView(view)
            .setPositiveButton("Fechar") { dialog, _ -> dialog.dismiss() }
            .create()
    }
}


